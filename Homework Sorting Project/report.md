# 41343110 and 41343153
# Homework Sorting Project
# Problem
## 解題說明
首先我們先了解本次作業的目標是由 

--插入排序

--快速排序

--歸併排序

--堆排序

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
在這次的程式中，我們在 Merge Sort 的基礎上實作了 Composite Sort。我們捨棄了把陣列遞迴切割到只剩 1 個元素的做法，而是加入了 threshold = 20 的門檻。這是因為當資料量切得很小時，頻繁的函式遞迴呼叫與記憶體配置反而會嚴重拖慢系統；這時直接切換成對小陣列極度高效的 Insertion Sort，能大幅削減遞迴深度，提升整體效能。此外，在 Quick Sort 的設計上，加入了 Median-of-Three（三數取中） 的防禦性寫法。這樣做可以有效避免在遇到「已經部分排序好」的極端測資時，時間複雜度退化成 $O(N^2)$ 甚至把堆疊（Stack）撐爆的風險。最後，在效能測量的數值處理上，為了避免「陣列複製」的時間干擾實驗結果，特別在計時器中跑了一組一模一樣的空迴圈來計算基礎 Overhead，並在最終結果中將其扣除。這個防呆機制確保了記錄下來的毫秒數，都是演算法純粹運算的時間，讓數據分析更具說服力。
### 結論
這次做完感覺演算法的理論複雜度真的只是基礎，實作細節跟硬體特性才是決定速度的關鍵。之前看教科書覺得 Quick Sort、Merge Sort 和 Heap Sort 平均都是 $O(N \log N)$ 應該差不多快，但實際跑過後，發現 Quick Sort 因為對快取記憶體比較友善，平均速度硬是比另外兩個快上不少；而 Merge Sort 雖然穩定，但必須不斷操作 tmp 暫存陣列，確實佔用了不少額外時間。特別是為了測試極限，我們寫了生成「最壞情況（Worst-case）」的測資函式去攻擊這些演算法，看著 Insertion Sort 的執行時間因為 $O(N^2)$ 暴增，對比 Heap Sort 依然穩如泰山，邏輯瞬間就變得非常具體。這次經驗也讓我們徹底明白，為什麼現代 C++ 標準函式庫底層的 std::sort 都不會死守單一演算法，而是像我的 Composite Sort 一樣，截長補短使用混合式排序了。
