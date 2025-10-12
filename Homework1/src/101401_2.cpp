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
    int stack[10000]; 
    int top = 0;        
    stack[top++] = m;   

    while (top > 0) {
        m = stack[--top];

        if (m == 0) {
            n = n + 1;     
        } else if (n == 0) {
            n = 1;        
            stack[top++] = m - 1;
        } else {
            stack[top++] = m - 1; 
            stack[top++] = m; 
            n = n - 1;
        }
    }
    return n;
}
int main() {
    int m, n;
    cin >> m >> n;
    cout << Akm(m, n) << endl;
    return 0;
}
