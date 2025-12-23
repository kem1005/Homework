# 41343153
# Homework3 
# Problem1

## 解題說明
本題要求實作一個多項式（Polynomial）類別，支援多項式的
輸入、輸出、加法、減法、乘法與帶入值計算，並需自行管理記憶體。

為了有效率地儲存多項式中的各項，本程式使用
鏈結串列（Linked List） 來表示多項式，每一個節點代表一項 (coef, exp)。
並透過 Iterator 來遍歷串列，使運算過程更直觀且模組化。

此外，為減少頻繁的動態記憶體配置成本，設計了
Available List（Free List） 機制，回收已釋放的節點供後續重複使用，
以提升整體效能並符合資料結構課程的設計精神。

## 程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

template<class T>
class ChainNode {
public:
    T element;
    ChainNode<T>* link;
    ChainNode(const T& e = T(), ChainNode<T>* l = nullptr):element(e), link(l) {}
};

template<class T>
class ChainIterator {
public:
    ChainNode<T>* current;
    ChainIterator(ChainNode<T>* start = nullptr) : current(start) {}
    T& operator*() const { return current->element; }
    T* operator->() const { return &current->element; }
    ChainIterator<T>& operator++() {
        current = current->link;
        return *this;
    }
    ChainIterator<T> operator++(int) {
        ChainIterator<T> old = *this;
        current = current->link;
        return old;
    }
    bool operator==(const ChainIterator<T>& rhs) const {
        return current == rhs.current;
    }
    bool operator!=(const ChainIterator<T>& rhs) const {
        return current != rhs.current;
    }
};

template<class T>
class Chain {
protected:
    ChainNode<T>* header;
public:
    typedef ChainIterator<T> iterator;
    Chain() {
        header = new ChainNode<T>();
        header->link = header;
    }
    ~Chain() { Release(); delete header; }
    bool IsEmpty() const {
        return header->link == header;
    }
    iterator Begin() const { return iterator(header->link); }
    iterator End() const { return iterator(header); }
    void Release() {
        ChainNode<T>* cur = header->link;
        while (cur != header) {
            ChainNode<T>* del = cur;
            cur = cur->link;
            delete del;
        }
        header->link = header;
    }
    void InsertBack(const T& e) {
        ChainNode<T>* cur = header;
        while (cur->link != header)
            cur = cur->link;
        cur->link = new ChainNode<T>(e, header);
    }
};

struct Term {
    int coef;
    int exp;
};

class AvailableList {
private:
    ChainNode<Term>* head;
public:
    AvailableList() : head(nullptr) {}
    bool IsEmpty() const { return head == nullptr; }
    void GetBack(ChainNode<Term>* node) {
        node->link = head;
        head = node;
    }
    ChainNode<Term>* GetNode() {
        if (IsEmpty()) return nullptr;
        ChainNode<Term>* node = head;
        head = head->link;
        node->link = nullptr;
        return node;
    }
};

class Polynomial : public Chain<Term> {
private:
    static AvailableList Ava;
    

public:
    Polynomial() : Chain<Term>() {}
    ~Polynomial() {
        ChainNode<Term>* cur = header->link;
        while (cur != header) {
            ChainNode<Term>* del = cur;
            cur = cur->link;
            Ava.GetBack(del);
        }
        header->link = header;
    }
    void NewTerm(int c, int e) {
        if (c == 0) return;
        ChainNode<Term>* prev = header;
        ChainNode<Term>* cur = header->link;
        while (cur != header && cur->element.exp > e) {
            prev = cur;
            cur = cur->link;
        }
        if (cur != header && cur->element.exp == e) {
            cur->element.coef += c;
            if (cur->element.coef == 0) {
                prev->link = cur->link;
                Ava.GetBack(cur);
            }
        }
        else {
            ChainNode<Term>* node = Ava.GetNode();
            if (!node) node = new ChainNode<Term>();
            node->element.coef = c;
            node->element.exp = e;
            node->link = cur;
            prev->link = node;
        }
    }
    float Evaluate(float x) const {
       float result = 0.0;
       for (auto it = Begin(); it != End(); ++it) { 
            result += it->coef * pow(x, it->exp);
        }
        return result;
    }   
    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        auto it1 = Begin();
        auto it2 = b.Begin();
        while (it1 != End() && it2 != b.End()) {
            if (it1->exp == it2->exp) {
                c.NewTerm(it1->coef + it2->coef, it1->exp);
                ++it1; ++it2;
            }
            else if (it1->exp > it2->exp) {
                c.NewTerm(it1->coef, it1->exp);
                ++it1;
            }
            else {
                c.NewTerm(it2->coef, it2->exp);
                ++it2;
            }
        }
        while (it1 != End()) {
            c.NewTerm(it1->coef, it1->exp);
            ++it1;
        }
        while (it2 != b.End()) {
            c.NewTerm(it2->coef, it2->exp);
            ++it2;
        }
        return c;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial c;
        auto it1 = Begin();
        auto it2 = b.Begin();
        while (it1 != End() && it2 != b.End()) {
            if (it1->exp == it2->exp) {
                c.NewTerm(it1->coef - it2->coef, it1->exp);
                ++it1; ++it2;
            }
            else if (it1->exp > it2->exp) {
                c.NewTerm(it1->coef, it1->exp);
                ++it1;
            }
            else {
                c.NewTerm(-it2->coef, it2->exp);
                ++it2;
            }
        }
        while (it1 != End()) {
            c.NewTerm(it1->coef, it1->exp);
            ++it1;
        }
        while (it2 != b.End()) {
            c.NewTerm(-it2->coef, it2->exp);
            ++it2;
        }
        return c;
    }
    

    Polynomial operator*(const Polynomial& b) const {
        Polynomial c;
        for (auto it1 = Begin(); it1 != End(); ++it1) {
            for (auto it2 = b.Begin(); it2 != b.End(); ++it2) {
                c.NewTerm(it1->coef * it2->coef, it1->exp + it2->exp);
            }
        }
        return c;
    }

    friend ostream& operator<< (ostream& os, const Polynomial& poly) {
    for (ChainIterator<Term> it = poly.Begin(); it != poly.End(); ++it) {
    if (it!=poly.Begin()) os << " + ";
    os << it->coef << "x^" << it->exp << " ";
    }
    return os;
    }

    friend istream& operator>>(istream& is, Polynomial& p) {
        int n, c, e;
        is >> n;
        for (int i = 0; i < n; i++) {
            is >> c >> e;
            p.NewTerm(c, e);
        }
        return is;
    }
};

AvailableList Polynomial::Ava;

int main() {
    Polynomial p1, p2;
    float x;
    cin >> p1 >> p2;
    cout << "x=";
    cin >> x;
    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;
    cout << "P1 + P2 = " << (p1 + p2) << endl;
    cout << "P1 - P2 = " << (p1 - p2) << endl;
    cout << "P1 * P2 = " << (p1 * p2) << endl;
    cout <<"P1=>"<<"P("<<x<<")="<<p1.Evaluate(x)<< endl;
    cout <<"P2=>"<<"P("<<x<<")="<<p2.Evaluate(x)<< endl;
    return 0;
}
```
## 效能分析

## 申論及開發報告
本程式以鏈結串列（Linked List）作為多項式的主要資料結構，每一個節點代表多項式中的一項，包含係數與指數兩個資訊。由於多項式的項數在執行期間可能動態變化，且各項指數不一定連續，使用鏈結串列能有效避免陣列需預先配置大小或頻繁搬移資料的缺點，提升結構彈性。
在記憶體管理方面，程式進一步設計了 Available List（Free List） 作為節點回收機制。綜合而言，本程式透過鏈結串列、Iterator 及 Available List 的搭配設計，在結構彈性、運算效率與記憶體管理之間取得良好平衡，成功實作一個功能完整且符合資料結構原則的多項式運算系統。
## 測試與驗證
### 測試輸入
```
3 3 3 3 2 2 0
第一個3是指有三項
3       3 3 3 2 2 0 = 3x3+3x2+2x
4 7 4 3 3 2 2 4 0
第一個4是指有四項
4       7 4 3 3 2 2 4 0 = 7x4+3x3+2x2+4x
x=2
```
### 測試輸出
```
P1 = 3x^3  + 3x^2  + 2x^0
P2 = 7x^4  + 3x^3  + 2x^2  + 4x^0
P1 + P2 = 7x^4  + 6x^3  + 5x^2  + 6x^0
P1 - P2 = -7x^4  + 1x^2  + -2x^0
P1 * P2 = 21x^7  + 30x^6  + 15x^5  + 20x^4  + 18x^3  + 16x^2  + 8x^0
P1=>P(2)=38
P2=>P(2)=148
```
### 結論
本程式實作一套以鏈結串列為基礎的多項式運算系統，支援多項式的輸入與輸出，以及加法、減法、乘法與帶入值計算等基本操作。透過將多項式各項依指數大小排序儲存，使得多項式運算能以線性方式完成，大幅提升運算效率。
在資料結構設計上，使用 Iterator 來遍歷串列，降低模組間的耦合度，使程式結構更具可讀性與維護性；同時搭配 Available List 進行節點回收，有效減少動態記憶體配置與釋放的次數，在不增加漸進空間複雜度的前提下，改善實際執行效能。
