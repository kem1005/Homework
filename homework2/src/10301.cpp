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

    cout << "加法結果: " << sum << endl;
    cout << "乘法結果: " << product << endl;
    return 0;
}
