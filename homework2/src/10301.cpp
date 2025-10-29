#include <iostream>
#include <cmath>
using namespace std;

class Term {
    friend class Polynomial;
public:
    float coef;
    int exp;
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
};

Polynomial::Polynomial(int cap) {
    capacity = cap;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

//輸入運算子
istream& operator>>(istream& in, Polynomial& p) {
    cout << "請輸入係數與指數（輸入 1010 結束）:\n";
    p.terms = 0;
    while (true) {
        float c;
        int e;
        in >> c;
        if (c == 1010) break;
        in >> e;
        p.termArray[p.terms].coef = c;
        p.termArray[p.terms].exp = e;
        p.terms++;
    }
    return in;
}


//輸出運算子
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

int main() {
    Polynomial p;
    cin >> p;
    cout << "= " << p << endl;
    return 0;
}
