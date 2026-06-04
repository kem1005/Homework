# 41343110 and 41343153
# Homework Sorting Project
# Problem
## 解題說明
## 程式實作
```
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// ---------- 各種排序法 ----------
void InsertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int x = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > x) arr[j + 1] = arr[j--];
        arr[j + 1] = x;
    }
}

int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
    return mid;
}
void QuickSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivotIdx = medianOfThree(arr, left, right);
    int pivot = arr[pivotIdx];
    swap(arr[pivotIdx], arr[right]);
    int i = left;
    for (int j = left; j < right; ++j) {
        if (arr[j] < pivot) swap(arr[i++], arr[j]);
    }
    swap(arr[i], arr[right]);
    QuickSort(arr, left, i - 1);
    QuickSort(arr, i + 1, right);
}
void QuickSort(vector<int>& arr) {
    QuickSort(arr, 0, arr.size() - 1);
}

void Merge(vector<int>& arr, int left, int mid, int right, vector<int>& tmp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        tmp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= right) tmp[k++] = arr[j++];
    for (int l = left; l <= right; ++l) arr[l] = tmp[l];
}
void MergeSort(vector<int>& arr) {
    int n = arr.size();
    vector<int> tmp(n);
    for (int sz = 1; sz < n; sz *= 2) {
        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}

void Heapify(vector<int>& arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        Heapify(arr, n, largest);
    }
}
void HeapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) Heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        Heapify(arr, i, 0);
    }
}

// ---------- 測試資料產生 ----------
vector<int> worstCaseInsertion(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = n - i;
    return arr;
}
vector<int> worstCaseMerge(int n) {
    vector<int> arr(n);
    int t = 1;
    for (int i = 0; i < n; ++i) arr[i] = t++;
    next_permutation(arr.begin(), arr.end());
    return arr;
}
vector<int> randomPermutation(int n, mt19937& rng) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    shuffle(arr.begin(), arr.end(), rng);
    return arr;
}

// ---------- 效能測試框架 ----------
typedef void(*SortFunc)(vector<int>&);
double testSort(SortFunc sortFunc, vector<int> arr, int repeat = 1) {
    clock_t start = clock();
    for (int i = 0; i < repeat; ++i) {
        vector<int> temp = arr;
        sortFunc(temp);
    }
    clock_t end = clock();
    double ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    return ms / repeat;
}

// ---------- 綜合 function ----------
void compositeSort(vector<int>& arr) {
    if (arr.size() < 20) {
        InsertionSort(arr);
    }
    else if (arr.size() < 1000) {
        QuickSort(arr);
    }
    else {
        MergeSort(arr);
    }
}

// ---------- 主程式樣板 ----------
int main() {
    vector<int> ns = { 500, 1000, 2000, 3000, 4000, 5000 };
    random_device rd;
    mt19937 rng(rd());
    int heapPerms = 10, averagePerms = 20;

    // 計時精度
    double clock_accuracy = testSort(InsertionSort, { 1,2,3,4,5 }, 10000);
    cout << "Clock accuracy: " << clock_accuracy << " ms" << endl;

    for (int n : ns) {
        // Worst-case
        double tIns = testSort(InsertionSort, worstCaseInsertion(n));
        double tQ = testSort(QuickSort, worstCaseInsertion(n));
        double tM = testSort(MergeSort, worstCaseMerge(n));
        double tH_max = 0;
        for (int i = 0; i < heapPerms; ++i) {
            double tH = testSort(HeapSort, randomPermutation(n, rng));
            tH_max = max(tH_max, tH);
        }
        double tC = testSort(compositeSort, worstCaseInsertion(n));
        cout << "n = " << n << " (worst-case ms)\n";
        cout << "Insertion: " << tIns << "\tQuick: " << tQ << "\tMerge: " << tM << "\tHeap: " << tH_max << "\tComposite: " << tC << endl;

        // Average-case
        double aIns = 0, aQ = 0, aM = 0, aH = 0, aC = 0;
        for (int i = 0; i < averagePerms; ++i) {
            auto arr = randomPermutation(n, rng);
            aIns += testSort(InsertionSort, arr);
            aQ += testSort(QuickSort, arr);
            aM += testSort(MergeSort, arr);
            aH += testSort(HeapSort, arr);
            aC += testSort(compositeSort, arr);
        }
        aIns /= averagePerms; aQ /= averagePerms; aM /= averagePerms; aH /= averagePerms; aC /= averagePerms;
        cout << "n = " << n << " (average-case ms)\n";
        cout << "Insertion: " << aIns << "\tQuick: " << aQ << "\tMerge: " << aM << "\tHeap: " << aH << "\tComposite: " << aC << endl;
    }
    return 0;
}
```
## 效能分析
#### 時間複雜度和空間複雜度
## 測試與驗證
### 測試輸出
## 申論及開發報告
### 結論
