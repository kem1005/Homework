#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
using namespace std;
int Akm(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return Akm(m - 1, 1);
    else
        return Akm(m - 1, Akm(m, n - 1));
}
int main() {
    int m, n;
    cin >> m >> n;
    cout << Akm(m, n) << endl;
    return 0;
}
