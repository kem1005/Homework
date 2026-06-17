# 41343110 and 41343153
# Homework Sorting Project
# Problem
## 解題說明
首先我們先了解本次作業的目標是由 

 - 插入排序
 - 快速排序
 - 歸併排序
 - 堆積排序
 - 複合排序 

這些去設計出一個複合排序函數，使其在最壞時間準則下表現良好。
## 程式實作
```
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std; 


// Insertion Sort 
void InsertionSortRange(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void InsertionSort(vector<int>& arr) {
    if (arr.empty()) return;
    InsertionSortRange(arr, 0, arr.size() - 1);
}

// Quick Sort 
int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
    return mid; 
}

void QuickSortHelper(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int pivotIdx = medianOfThree(arr, left, right);
    int pivotVal = arr[pivotIdx]; 


    swap(arr[pivotIdx], arr[right - 1]);

    int i = left;
    int j = right - 1;

    while (true) {
        while (arr[++i] < pivotVal) {}
        while (arr[--j] > pivotVal) {}
        if (i < j) {
            swap(arr[i], arr[j]);
        }
        else {
            break;
        }
    }
 
    swap(arr[i], arr[right - 1]);

    QuickSortHelper(arr, left, i - 1);
    QuickSortHelper(arr, i + 1, right);
}

void QuickSort(vector<int>& arr) {
    if (arr.size() <= 1) return;
    QuickSortHelper(arr, 0, arr.size() - 1);
}

// Merge Sort 
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
    if (n <= 1) return;
    vector<int> tmp(n);
    for (int sz = 1; sz < n; sz *= 2) {
        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}

// Heap Sort (Max-Heap)
void Heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        Heapify(arr, n, largest);
    }
}
// Heap Sort
void HeapSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    for (int i = n / 2 - 1; i >= 0; --i) Heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        Heapify(arr, i, 0); 
    }
}

// Composite Sort function 
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    int threshold = 20;
    vector<int> tmp(n);

    for (int sz = 1; sz < n; sz *= 2) {
        if (sz < threshold) {
            for (int left = 0; left < n; left += 2 * sz) {
                int right = min(left + 2 * sz - 1, n - 1);
                InsertionSortRange(arr, left, right);
            }
            continue; 
        }

        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}


// make a test 
vector<int> worstCaseInsertion(int n) {
    vector<int> arr(n);    
    for (int i = 0; i < n; ++i) arr[i] = n - i;
    return arr;
}

void generateMergeWorst(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    if (right - left == 1) {
        swap(arr[left], arr[right]);
        return;
    }
    vector<int> tmp(right - left + 1);
    int mid = left + (right - left) / 2;
    int lIdx = 0, rIdx = (mid - left + 1);

    for (int i = left; i <= right; i += 2) {
        tmp[lIdx++] = arr[i];
        if (i + 1 <= right) tmp[rIdx++] = arr[i + 1];
    }
    for (int i = left; i <= right; ++i) arr[i] = tmp[i - left];

    generateMergeWorst(arr, left, mid);
    generateMergeWorst(arr, mid + 1, right);
}

vector<int> worstCaseMerge(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    generateMergeWorst(arr, 0, n - 1);
    return arr;
}

void Permute(vector<int>& arr) {
    int n = arr.size();
    for (int i = n - 1; i >= 1; i--) {
        int j = rand() % (i + 1);
        swap(arr[j], arr[i]);
    }
}

vector<int> randomPermutation(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    Permute(arr);
    return arr;
}


typedef void(*SortFunc)(vector<int>&);

double testSort(SortFunc sortFunc, const vector<int>& baseArr, int repeat = 1) {
    clock_t startTotal = clock();
    for (int i = 0; i < repeat; ++i) {
        vector<int> temp = baseArr; 
        sortFunc(temp);             
    }
    clock_t endTotal = clock();

    clock_t startOverhead = clock();
    for (int i = 0; i < repeat; ++i) {
        vector<int> temp = baseArr; 
    }
    clock_t endOverhead = clock();

    long totalTicks = (endTotal - startTotal) - (endOverhead - startOverhead);
    if (totalTicks < 0) totalTicks = 0;

    double ms = 1000.0 * totalTicks / CLOCKS_PER_SEC;
    return ms / repeat;
}


int main() {
    srand(2026);

    vector<int> ns = { 500, 1000, 2000, 3000, 4000, 5000 };
    int heapPerms = 15;    
    int averagePerms = 20; 

  
    cout << " 最壞狀況分析 (Worst-case Runtimes in ms)\n";
   

    for (int n : ns) {
        int r = (n <= 1000) ? 200 : 20;

        vector<int> insWorst = worstCaseInsertion(n);
        vector<int> mergeWorst = worstCaseMerge(n);

        double tIns = testSort(InsertionSort, insWorst, r);
        double tM = testSort(MergeSort, mergeWorst, r);
        double tC = testSort(compositeSort, insWorst, r);

        double tQ_max = 0;
        double tH_max = 0;
        for (int i = 0; i < heapPerms; ++i) {
            vector<int> randArr = randomPermutation(n);
            tQ_max = max(tQ_max, testSort(QuickSort, randArr, r));
            tH_max = max(tH_max, testSort(HeapSort, randArr, r));
        }

        cout << "n = " << n << " ->\n";
        cout << "  Insertion: " << tIns << " ms\tQuick(Max): " << tQ_max << " ms\tMerge: " << tM 
             << " ms\tHeap(Max): " << tH_max << " ms\tComposite: " << tC << " ms\n\n";
    }

    cout << "平均狀況分析 (Average Runtimes in ms)\n";

    for (int n : ns) {
        double aIns = 0, aQ = 0, aM = 0, aH = 0, aC = 0;
        int r = (n <= 1000) ? 50 : 5; 

        for (int i = 0; i < averagePerms; ++i) {
            vector<int> randArr = randomPermutation(n);

            aIns += testSort(InsertionSort, randArr, r);
            aQ += testSort(QuickSort, randArr, r);
            aM += testSort(MergeSort, randArr, r);
            aH += testSort(HeapSort, randArr, r);
            aC += testSort(compositeSort, randArr, r);
        }

        cout << "n = " << n << " -> \n";
        cout << "  Insertion: " << aIns / averagePerms << " ms\tQuick: " << aQ / averagePerms
             << " ms\tMerge: " << aM / averagePerms << " ms\tHeap: " << aH / averagePerms
             << " ms\tComposite: " << aC / averagePerms << " ms\n\n";
    }

    return 0;
}
```
### 片段程式碼的功能與邏輯：
#### 插入排序 (Insertion Sort)
```
void InsertionSortRange(vector<int> &arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void InsertionSort(vector<int> &arr)
{
    if (arr.empty())
        return;
    InsertionSortRange(arr, 0, arr.size() - 1);
}
```
- InsertionSortRange：這是插入排序的核心。它允許你只對陣列中的某個指定範圍 [left, right] 進行排序（這對後面的複合排序很有用）。基本原理是每次拿一個數字（key），往左邊已經排好序的隊伍裡面比較，把比它大的數字往右移，直到找到合適的位置放進去。

- InsertionSort：對外的包裹函式（Wrapper）。先檢查陣列是否為空，如果不為空，就呼叫上面的函式，把整個陣列 [0, size-1] 進行排序。
#### 快速排序 (Quick Sort)
```
int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
    return mid; 
}
```
- medianOfThree（三數取中法）：快速排序最怕選到極端值（最大或最小）當基準點（Pivot），那會讓時間複雜度劣化到 $O(n^2)$。這個函式取陣列的「最左」、「中間」、「最右」三個數進行比較與交換，確保中間位置（mid）的數是這三個數的中位數，並回傳這個位置。這能大幅優化快速排序的效能。
```
void QuickSortHelper(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int pivotIdx = medianOfThree(arr, left, right);
    int pivotVal = arr[pivotIdx]; 

    swap(arr[pivotIdx], arr[right - 1]); // 將基準點藏到 right - 1 的位置

    int i = left;
    int j = right - 1;

    while (true) {
        while (arr[++i] < pivotVal) {} // i 往右找比 pivot 大的
        while (arr[--j] > pivotVal) {} // j 往左找比 pivot 小的
        if (i < j) {
            swap(arr[i], arr[j]);
        }
        else {
            break;
        }
    }
 
    swap(arr[i], arr[right - 1]); // 把基準點換回正確的分割點

    QuickSortHelper(arr, left, i - 1);  // 遞迴排序左半邊
    QuickSortHelper(arr, i + 1, right); // 遞迴排序右半邊
}

void QuickSort(vector<int>& arr) {
    if (arr.size() <= 1) return;
    QuickSortHelper(arr, 0, arr.size() - 1);
}
```
- QuickSortHelper：快速排序的核心（採用了 Hoare Partition 的變形）。

1 利用三數取中法選出基準點的值 pivotVal。

2 使用雙指針 i 和 j 從兩端向中間掃描，把小於基準點的丟左邊，大於基準點的丟右邊。

3 分割完成後，再用遞迴（Divide and Conquer）分別對左、右兩子陣列進行同樣的處理。
#### 合併排序 (Merge Sort)
```
void Merge(vector<int>& arr, int left, int mid, int right, vector<int>& tmp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        tmp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= right) tmp[k++] = arr[j++];
    for (int l = left; l <= right; ++l) arr[l] = tmp[l];
}
```
- Merge：負責將兩個已經排好序的子陣列（[left, mid] 與 [mid+1, right]）合併成一個更大的、有序的陣列。它利用一個輔助陣列 tmp 來暫存合併後的結果，最後再複製回原陣列 arr。
```
  void MergeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    vector<int> tmp(n);
    for (int sz = 1; sz < n; sz *= 2) {
        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}
```
- MergeSort：這裡實作的是非遞迴（Bottom-up，自底向上）的合併排序。它從大小為 1 的子陣列開始，兩兩合併成大小為 2 的有序陣列；接著大小變成 2, 4, 8... 依此類推，直到整個陣列都合併完成。這避免了遞迴帶來的系統堆疊（Stack）開銷。
#### 堆積排序 (Heap Sort)
```
void Heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1; // 左子節點索引
    int r = 2 * i + 2; // 右子節點索引
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        Heapify(arr, n, largest); // 繼續向下調整
    }
}
```
- Heapify：維護「最大堆積（Max-Heap）」特性的核心函式。假設某個節點 i 的左右子樹都已經是最大堆積，若 i 本身的值比子節點小，就將它與較大的子節點交換，並繼續向下調整，直到節點 i 滿足「父節點大於等於子節點」的特性。
```
void HeapSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    // 步驟 1：建立最大堆積
    for (int i = n / 2 - 1; i >= 0; --i) Heapify(arr, n, i);
    // 步驟 2：逐步取出最大值並調整
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]); // 將最大值（根節點）換到陣列尾端
        Heapify(arr, i, 0);   // 重新調整賸餘的陣列為最大堆積
    }
}
```
- HeapSort：

1 建堆：從最後一個非葉子節點開始往上呼叫 Heapify，把無序陣列變成一個最大堆積。此時 arr[0] 必定是最大值。

2 排序：將 arr[0] 與目前堆積的最後一個元素交換，然後縮小堆積的範圍（i 減少），再對 arr[0] 進行 Heapify。重覆這個過程，就能將最大值依序放到陣列後方，達成排序。
#### 複合排序 (Composite Sort)
```
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    int threshold = 20;
    vector<int> tmp(n);

    for (int sz = 1; sz < n; sz *= 2) {
        if (sz < threshold) {
            for (int left = 0; left < n; left += 2 * sz) {
                int right = min(left + 2 * sz - 1, n - 1);
                InsertionSortRange(arr, left, right);
            }
            continue; 
        }

        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}
```
- compositeSort：這是一個混合型演算法（類似 TimSort 或內省排序的某些概念）。
- 邏輯：它修改了 Bottom-up Merge Sort 的邏輯。當子陣列的長度很小（sz < threshold，這裡設定為 20）時，程式認為此時用 $O(n^2)$ 但常數極小的插入排序效率更好，因此直接對該區間進行插入排序。
- 當子陣列長度大於或等於 20 後，才開始切換成標準的合併排序（Merge）。
## 效能分析
#### 時間複雜度和
先了解在不同的排序方法中，每個的時間複雜度在平均和最壞的情況下為多少。
| Sorting                  | 平均                 | 最壞                                           |
|-----------------------|-----------------------------|---------------------------|
|Insertion Sort|$O(n^2)$|$O(n^2)$|
|Quick Sort|$O(n \log n)$|$O(n^2)$|
|Merge Sort|$O(n \log n)$| $O(n \log n)$|
|Heap Sort| $O(n \log n)$|$O(n \log n)$|

## 測試與驗證
### 最壞狀況分析 (Worst-case Runtimes in ms)
當資料量為 n筆，且資料處於「最壞狀況」時，各演算法耗費的毫秒數 (ms)。
| 順序  |  輸出     |
|----|-------------------------|
| 一 |n = 500 ->    Insertion: 0.762705 ms    Quick(Max): 0.04186 ms    Merge: 0.06299 ms    Heap(Max): 0.090035 ms    Composite: 0.0864 ms|
| 二 |n = 1000 ->    Insertion: 3.1151 ms    Quick(Max): 0.110325 ms    Merge: 0.131255 ms    Heap(Max): 0.28958 ms    Composite: 0.2038 ms|
| 三 |n = 2000 ->    Insertion: 13.8283 ms    Quick(Max): 0.17235 ms    Merge: 0.291 ms    Heap(Max): 0.62305 ms    Composite: 0.3817 ms|
| 四 |n = 3000 ->    Insertion: 27.705 ms    Quick(Max): 0.4803 ms    Merge: 0.4731 ms    Heap(Max): 1.36185 ms    Composite: 0.60685 ms|
| 五 |n = 4000 ->    Insertion: 53.8867 ms    Quick(Max): 0.68665 ms    Merge: 0.63035 ms    Heap(Max): 1.6519 ms    Composite: 0.7839 ms|
| 六|n = 5000 ->    Insertion: 79.9134 ms    Quick(Max): 0.81555 ms    Merge: 0.81 ms    Heap(Max): 2.2117 ms    Composite: 1.00615 ms|

### 平均狀況分析 (Average Runtimes in ms) 
當資料量為 n 筆，且資料處於「平均狀況」時，各演算法耗費的毫秒數 (ms)。
| 順序  |  輸出     | 
|----|-------------------------|
| 一 |n = 500 ->     Insertion: 0.473159 ms    Quick: 0.039284 ms    Merge: 0.071288 ms    Heap: 0.100513 ms    Composite: 0.08932 ms|
| 二 |n = 1000 ->     Insertion: 1.61248 ms    Quick: 0.077313 ms    Merge: 0.170585 ms    Heap: 0.232328 ms    Composite: 0.178115 ms|
| 三 |n = 2000 ->     Insertion: 6.40731 ms    Quick: 0.21554 ms    Merge: 0.40593 ms    Heap: 0.58731 ms    Composite: 0.42767 ms|
| 四 |n = 3000 ->     Insertion: 15.8107 ms    Quick: 0.41105 ms    Merge: 0.72026 ms    Heap: 0.99519 ms    Composite: 0.79802 ms|
| 五 |n = 4000 ->     Insertion: 26.497 ms    Quick: 0.57002 ms    Merge: 0.9568 ms    Heap: 1.38242 ms    Composite: 0.99852 ms|
| 六 |n = 5000 ->     Insertion: 41.9723 ms    Quick: 0.72999 ms    Merge: 1.26124 ms    Heap: 1.80986 ms    Composite: 1.35229 ms|
### 測試輸出
```
最壞狀況分析 (Worst-case Runtimes in ms)
n = 500 ->
  Insertion: 0.762705 ms    Quick(Max): 0.04186 ms    Merge: 0.06299 ms    Heap(Max): 0.090035 ms    Composite: 0.0864 ms

n = 1000 ->
  Insertion: 3.1151 ms    Quick(Max): 0.110325 ms    Merge: 0.131255 ms    Heap(Max): 0.28958 ms    Composite: 0.2038 ms

n = 2000 ->
  Insertion: 13.8283 ms    Quick(Max): 0.17235 ms    Merge: 0.291 ms    Heap(Max): 0.62305 ms    Composite: 0.3817 ms

n = 3000 ->
  Insertion: 27.705 ms    Quick(Max): 0.4803 ms    Merge: 0.4731 ms    Heap(Max): 1.36185 ms    Composite: 0.60685 ms

n = 4000 ->
  Insertion: 53.8867 ms    Quick(Max): 0.68665 ms    Merge: 0.63035 ms    Heap(Max): 1.6519 ms    Composite: 0.7839 ms

n = 5000 ->
  Insertion: 79.9134 ms    Quick(Max): 0.81555 ms    Merge: 0.81 ms    Heap(Max): 2.2117 ms    Composite: 1.00615 ms

平均狀況分析 (Average Runtimes in ms)
n = 500 -> 
  Insertion: 0.473159 ms    Quick: 0.039284 ms    Merge: 0.071288 ms    Heap: 0.100513 ms    Composite: 0.08932 ms

n = 1000 -> 
  Insertion: 1.61248 ms    Quick: 0.077313 ms    Merge: 0.170585 ms    Heap: 0.232328 ms    Composite: 0.178115 ms

n = 2000 -> 
  Insertion: 6.40731 ms    Quick: 0.21554 ms    Merge: 0.40593 ms    Heap: 0.58731 ms    Composite: 0.42767 ms

n = 3000 -> 
  Insertion: 15.8107 ms    Quick: 0.41105 ms    Merge: 0.72026 ms    Heap: 0.99519 ms    Composite: 0.79802 ms

n = 4000 -> 
  Insertion: 26.497 ms    Quick: 0.57002 ms    Merge: 0.9568 ms    Heap: 1.38242 ms    Composite: 0.99852 ms

n = 5000 -> 
  Insertion: 41.9723 ms    Quick: 0.72999 ms    Merge: 1.26124 ms    Heap: 1.80986 ms    Composite: 1.35229 ms
```
## 申論及開發報告
### 結論
