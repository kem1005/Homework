#include <iostream>
using namespace std;

class Term {
    friend class Polynomial;
public:
    float coef; // �Y��
    int exp;    // ����
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
    Polynomial operator+(const Polynomial& b) const; // �h�����[�k
};

Polynomial::Polynomial(int cap) {
    capacity = cap;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

// ��J�B��l >>
istream& operator>>(istream& in, Polynomial& p) {
    cout << "�п�J�Y�ƻP���ơ]��J 1010 �����^:\n";
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

// ��X�B��l <<
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

// �h�����[�k
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result(capacity + b.capacity);
    int a = 0, b2 = 0;

    while (a< terms && b2 < b.terms) {
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

    for (; a < terms; a++)
        result.termArray[result.terms++] = termArray[a];
    for (; b2 < b.terms; b2++)
        result.termArray[result.terms++] = b.termArray[b2];

    return result;
}

int main() {
    Polynomial p1, p2, p3;
    cout << "��J�Ĥ@�Ӧh�����G" << endl;
    cin >> p1;
    cout << "��J�ĤG�Ӧh�����G" << endl;
    cin >> p2;

    p3 = p1 + p2;

    cout << "= " << p3 << endl;
    return 0;
}
