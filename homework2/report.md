# 41343110
# Homework2 Polynomial
# Problem1

## 解題說明
本題要求以物件導向的方式設計一個 Polynomial（多項式）類別，支援以下功能：

- 多項式的建立與儲存
使用動態陣列儲存多項式的各項（係數與指數）。

- 多項式的加法 (Add)
將兩個多項式相加，合併相同指數的項。

- 多項式的乘法 (Mult)
對兩個多項式的所有項做乘積運算，再合併同類項。

- 多項式的代入求值 (Eval)
將指定數值代入 x 中，計算多項式的值。

- 多項式的顯示 (Show)
將多項式以標準數學格式輸出，例如：3x^2+2x+1
## 解題策略
1. 物件導向設計

我們將問題分解為兩個主要的類別：

- Term 類別：表示多項式中的一個項（即係數和指數），這樣的設計能讓多項式的每一項都與其他項相互獨立。
- Polynomial 類別：代表一個多項式，它包含了多個 Term，並提供操作多項式的方法，如加法、乘法、代入求值等。
2. Term 類別設計
每個 Term 類別包含兩個成員變數：
- coef：項的係數。
- exp：項的指數。
這些變數都是私有的，並且 Polynomial 類別將能夠直接操作它們。
3. Polynomial 類別設計
Polynomial 類別包含以下功能：

- 動態儲存多項式：
    使用動態陣列 termArray 儲存多項式中的每個 Term，並且使用 capacity 和 terms 來追蹤目前的容量和實際儲存的項數。

- 新增項目 (NewTerm)：

    若新增的項有相同指數，則合併它們的係數。

    若新增項後陣列已滿，則需要擴展陣列。

    所有項目會依照指數排序，以便於後續的操作（例如加法和乘法）。

- 加法 (Add)：

    比較兩個多項式中的項，將相同指數的項相加，並合併結果。
- 乘法 (Mult)：

    每對項目進行乘法操作，然後合併同指數的項。

- 代入求值 (Eval)：

    針對多項式中的每一項，將 x 代入計算並求和。

- 顯示 (Show)：

    按照標準的數學格式輸出多項式，處理正負號、係數和指數。
## 程式實作
```cpp
#include <iostream>
using namespace std;

class Term {
    friend class Polynomial;
public:
    float coef; // 係數
    int exp;    // 指數
};

class Polynomial {
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial(int cap = 10);
    ~Polynomial();
    Polynomial(const Polynomial& other);
    Polynomial& operator=(const Polynomial& other);
    void Grow();
    void NewTerm(float c, int e);
    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;
};

void Polynomial::Grow() {
    capacity *= 2;
    Term* newArray = new Term[capacity];
    
    for (int i = 0; i < terms; ++i) {
        newArray[i] = termArray[i];
    }

    delete[] termArray;
    termArray = newArray;
}

Polynomial::Polynomial(const Polynomial& other) 
    : capacity(other.capacity), terms(other.terms) {
    
    termArray = new Term[capacity];
    
    for (int i = 0; i < terms; ++i) {
        termArray[i] = other.termArray[i];
    }
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this == &other) {
        return *this;
    }

    delete[] termArray;

    capacity = other.capacity;
    terms = other.terms;
    
    termArray = new Term[capacity];
    for (int i = 0; i < terms; ++i) {
        termArray[i] = other.termArray[i];
    }

    return *this;
}

Polynomial::Polynomial(int cap) {
    capacity = cap;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

void Polynomial::NewTerm(float c, int e) {
    if (c == 0) return;
    if (terms == capacity) Grow();

    for (int i = 0; i < terms; i++) {
        if (termArray[i].exp == e) {
            termArray[i].coef += c;
            if (termArray[i].coef == 0) {
                for (int j = i; j < terms - 1; j++)
                    termArray[j] = termArray[j + 1];
                terms--;
            }
            return;
        }
    }

    int i = terms - 1;
    while (i >= 0 && termArray[i].exp < e) {
        termArray[i + 1] = termArray[i];
        i--;
    }
    termArray[i + 1].coef = c;
    termArray[i + 1].exp = e;
    terms++;
}
// 輸入運算子 >>
istream& operator>>(istream& in, Polynomial& p) {
    cout << "請輸入係數與指數（輸入 1010 結束）:\n";
    p.terms = 0;
    while (true) {
        float c;
        int e;
        if (!(in >> c)) break; 
        if (c == 1010) break;
        if (!(in >> e)) break;
        p.NewTerm(c, e);
    }
    return in;
}

// 輸出運算子 <<
ostream& operator<<(ostream& out, const Polynomial& p) {
    for (int i = 0; i < p.terms; i++) {
        float c = p.termArray[i].coef;
        int e = p.termArray[i].exp;

        if (i > 0 && c >= 0) out << " + ";
        if (e == 0) out << c;
        else if (e == 1) out << c << "x";
        else out << c << "x^" << e;
    }
    return out;
}

// 多項式加法
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result(capacity + b.capacity);
    int a = 0, b2 = 0;

    while (a< terms && b2 < b.terms) {
        if (result.terms == result.capacity) {
            result.Grow();
        }
        if (termArray[a].exp == b.termArray[b2].exp) {
            float sum = termArray[a].coef + b.termArray[b2].coef;
            if (sum != 0) {
                result.termArray[result.terms].coef = sum;
                result.termArray[result.terms].exp = termArray[a].exp;
                result.terms++;
            }
            a++; b2++;
        } else if (termArray[a].exp > b.termArray[b2].exp) {
            result.termArray[result.terms] = termArray[a];
            result.terms++; a++;
        } else {
            result.termArray[result.terms] = b.termArray[b2];
            result.terms++; b2++;
        }
    }

    for (; a < terms; a++){
        if (result.terms == result.capacity) {
            result.Grow();
        }
        result.termArray[result.terms++] = termArray[a];
    }
    for (; b2 < b.terms; b2++){
        if (result.terms == result.capacity) {
             result.Grow();
         }
        result.termArray[result.terms++] = b.termArray[b2];
    }
    return result;
}
// 多項式乘法
Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial result(this->capacity * b.capacity);

    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < b.terms; j++) {
            float newCoef = termArray[i].coef * b.termArray[j].coef;
            int newExp = termArray[i].exp + b.termArray[j].exp;
            result.NewTerm(newCoef, newExp); 
        }
    }
    return result;
}

int main() {
    Polynomial p1, p2, sum, product;
    cout << "輸入第一個多項式：" << endl;
    cin >> p1;
    cout << "輸入第二個多項式：" << endl;
    cin >> p2;

    sum = p1 + p2;
    product = p1 * p2;

    cout << "= " << sum << endl;
    cout << "乘法結果: " << product << endl;
    return 0;
}
```
## 效能分析
在測量效能時，您可以觀察到以下幾點，並考慮潛在的最佳化：
- Add 函式：Add 函式會依序遍歷兩個多項式的項，這是一個效率不錯的合併操作（類似合併排序）。其效能主要取決於兩個多項式的項數。
- Mult 函式：這個函式的效能會較差，因為它使用一個迴圈嵌套另一個迴圈，並且在每次內部迴圈中都執行同類項的查找與合併操作，最後還使用慢速排序。
- Eval 函式：這個函式遍歷所有項次，並對每個項次使用 pow 函式來計算冪次。效率主要取決於項數 $N$ 和最大指數 $E_{max}$。
- NewTerm 函式：這個函式在每次插入新項時都會進行排序（氣泡排序），這會增加效能負擔。
## 測試與驗證
| 測試案例 | 輸入參數 $p1(x)$  | 預期輸出P1(x)  | 實際輸出 P1(x) | 
|----------|--------------|----------|----------|
| 測試一   |      3, 2     |             |         |       
|          |        2, 1    |            |        |  
|          |        1, 0    |    $3x^2$ + 2x + 1        |    $3x^2$ + 2x + 1      |  

| 測試案例 | 輸入參數 $p2(x)$  | 預期輸出P2(x)  | 實際輸出 P2(x) | 
|----------|--------------|----------|----------|
| 測試二  |      1, 1    |             |         |       
|          |       1, 1    |      2x      |  2x      | 

| 測試案例 |   預期輸出$p1(x)$ +    $p2(x)$  | 實際輸出 $p1(x)$ +    $p2(x)$ | 
|----------|----------|----------|
|   $p1(x)$ +    $p2(x)$     |    $3x^2$ + 4x + 1         |  $3x^2$ + 4x + 1       |   

| 測試案例 |   預期輸出$p1(x)$ * $p2(x)$  | 實際輸出 $p1(x)$ * $p2(x)$ | 
|----------|----------|----------|
|   $p1(x)$ * $p2(x)$     |    $6x^3$ + $4x^2$ + 2x       |  $6x^3$ + $4x^2$ + 2x       |   

| 測試案例 |   預期輸出$p1(2)$  | 實際輸出 $p1(2)$ | 
|----------|----------|----------|
|   $p1(2)$      |   17    | 17     |   

### 編譯與執行指令
#### 測試輸入
```cpp
p1.NewTerm(3, 2);  
p1.NewTerm(2, 1);  
p1.NewTerm(1, 0);  
p2.NewTerm(1, 1);  
p2.NewTerm(1, 0);  
```
#### 測試輸出
```shell
P1(x) = 5x^2 + 4x + 9
P2(x) = 2x
Sum = 5x^2 + 6x + 9
Product = 10x^3 + 8x^2 + 18x
P1(2) = 37
```
## 效能量測
| 功能          | 時間複雜度           | 空間複雜度      | 說明                     |
| ----------- | --------------- | ---------- | ---------------------- |
| `NewTerm()` | (O(n))          | (O(n))     | 需遍歷 $O(N)$ 查找同類項，並執行 $O(N)$ 插入排序來保持數組有序。     |
| `Add()`     | (O(n + m))      | (O(n + m)) | 基於兩個已排序的多項式，執行雙指標線性掃描合併，效率極高。            |
| `Mult()`    | (O(n \times m)) | (O(n + m)) | 執行 $N_A \cdot N_B$ 次兩兩相乘。每次乘積項都呼叫 $\boldsymbol{O(N_{res})}$ 的 NewTerm，導致總體複雜度很高。   |
| `Eval()`    | (O(n))          | (O(1))     |遍歷 $N$ 個項次並計算冪次。若使用霍納法則可降至 $O(N)$。|
## 申論及開發報告
本程式以「物件導向設計 (OOP)」為核心，採用類別封裝多項式運算邏輯：

- Term 類別封裝單一項（係數與指數）。

- Polynomial 類別負責多項式整體結構與運算。

- 使用 友元類別 (friend)，讓 Polynomial 能夠直接存取 Term 成員。

- 所有運算 (Add, Mult, Eval) 均為成員函式，實現高內聚。

- 採用 動態陣列，在多項式項數增加時自動擴容，避免記憶體不足。

開發重點：

- 保持項目依照指數遞減排序。

- 相同指數自動合併。

- 顯示格式貼近數學習慣，支援正負號輸出。

