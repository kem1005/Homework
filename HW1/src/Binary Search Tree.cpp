#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}

int height(Node* root) {
    if (!root) return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return 1 + (hl > hr ? hl : hr);
}

void destroy(Node* root) {
    if (!root) return;
    destroy(root->left);
    destroy(root->right);
    delete root;
}

void shuffleArray(int* a, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

int main() {
    srand((unsigned)time(nullptr)); 

    int ns[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
    int len = sizeof(ns) / sizeof(ns[0]);

    cout << left << setw(10) << "n" 
         << setw(10) << "height" 
         << "height/log2(n)" << endl;
    cout << "---------------------------------------" << endl;

    for (int i = 0; i < len; ++i) {
        int n = ns[i];
        Node* root = nullptr;

        int* arr = new int[n];
        for (int j = 0; j < n; ++j) arr[j] = j + 1;

        shuffleArray(arr, n);
        for (int j = 0; j < n; ++j) {
            root = insert(root, arr[j]);
        }

        int h = height(root);
        double ratio = h / log2((double)n);

        cout << left << setw(10) << n 
             << setw(10) << h 
             << fixed << setprecision(4) << ratio << "\n";

        delete[] arr;
        destroy(root);
    }

    return 0;
}