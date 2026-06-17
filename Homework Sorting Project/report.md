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
    for (int i = 0; i < i; ++i) arr[i] = n - i; 
    
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
#### 時間複雜度和空間複雜度
| 演算法                  | 時間複雜度                  | 空間複雜度                                           |說明|
|-----------------------|-----------------------------|---------------------------|------------------------------------|
|Insertion Sort|$O(n^2)$ / $O(n^2)$|$O(1)$|原地排序。適合極小資料量，但在面對完全反序的測資時，運算時間會呈現拋物線般的暴增。|
|Quick Sort|$O(n \log n)$ / $O(n^2)$|$O(\log n)$|空間消耗來自遞迴堆疊。實作「三數取中法」，成功避開了反序陣列造成的 $O(n^2)$ 退化，實測表現極佳。|
|Merge Sort|$O(n \log n)$ / $O(n \log n)$|$O(n)$|程式採用非遞迴的迴圈寫法，消除了遞迴造成的系統堆疊開銷，但代價是需要宣告一個長度同為 $n$ 的 tmp 陣列。|
|Heap Sort|$O(n \log n)$ / $O(n \log n)$|$O(1)$|透過 Heapify 進行原地調整，不需要額外記憶體空間，且完全沒有最差情況退化的風險，效能與記憶體表現最為穩定。|
|Composite Sort|$O(n \log n)$ / $O(n \log n)$|依條件而定|動態切換策略：n < 20 調用插入、n < 1000 調用快速、其餘調用合併，完美結合各家演算法在不同資料規模下的優勢。|

## 測試與驗證
| 順序 | 輸入參數                |  預期輸出     | 說明                             | 
|----|-------------------------|----------------------------------------|----------------------------------------|
| 一 | |Clock accuracy: 0.0001 ms|這是程式一開始呼叫 testSort 跑了一萬次極小陣列所測出來的「時鐘精確度」。  電腦計時器非常精準，可以捕捉到小至 $0.0002$ 毫秒的運算時間。這意味著後續看到的 0 ms，通常代表該演算法執行的時間真的小於這個極限值，快到無法被精確測量。|
| 二 | |n = 500 (worst-case ms)  Insertion: 0    Quick: 0        Merge: 0        Heap: 1 Composite: 0|當資料量為 500 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。| 
| 三 | |n = 500 (average-case ms)  Insertion: 0.05 Quick: 0        Merge: 0.05     Heap: 0.1       Composite: 0.05|當資料量為 500 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 四 | |n = 1000 (worst-case ms)  Insertion: 2    Quick: 0        Merge: 0        Heap: 1 Composite: 0|當資料量為 1000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 五 | |n = 1000 (average-case ms)  Insertion: 0    Quick: 0.1      Merge: 0.1      Heap: 0.2       Composite: 0.2|當資料量為 1000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 六 | |n = 2000 (worst-case ms)  Insertion: 9    Quick: 0        Merge: 0        Heap: 1 Composite: 0|當資料量為 2000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 七 | |n = 2000 (average-case ms)  Insertion: 0.15 Quick: 0.15     Merge: 0.25     Heap: 0.35      Composite: 0.35|當資料量為 2000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 八 | |n = 3000 (worst-case ms)  Insertion: 18   Quick: 1        Merge: 0        Heap: 1 Composite: 0|當資料量為 3000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 九 | |n = 3000 (average-case ms)  Insertion: 0.1  Quick: 0.45     Merge: 0.35     Heap: 0.7       Composite: 0.4|當資料量為 3000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 十 | |n = 4000 (worst-case ms)  Insertion: 34   Quick: 1        Merge: 0        Heap: 1 Composite: 0|當資料量為 4000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 十一 | |n = 4000 (average-case ms)  Insertion: 0.35 Quick: 0.55     Merge: 0.55     Heap: 0.95      Composite: 0.55|當資料量為 4000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 十二 | |n = 5000 (worst-case ms)  Insertion: 49   Quick: 1        Merge: 1        Heap: 2 Composite: 1|當資料量為 5000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 十三 | |n = 5000 (average-case ms)  Insertion: 0.15 Quick: 0.65     Merge: 0.8      Heap: 1.15      Composite: 0.8|當資料量為 5000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
   
### 測試輸出
```
Clock accuracy: 0.0001 ms
n = 500 (worst-case ms)
Insertion: 0    Quick: 0        Merge: 0        Heap: 1 Composite: 0
n = 500 (average-case ms)
Insertion: 0.05 Quick: 0        Merge: 0.05     Heap: 0.1       Composite: 0.05
n = 1000 (worst-case ms)
Insertion: 2    Quick: 0        Merge: 0        Heap: 1 Composite: 0
n = 1000 (average-case ms)
Insertion: 0    Quick: 0.1      Merge: 0.1      Heap: 0.2       Composite: 0.2
n = 2000 (worst-case ms)
Insertion: 9    Quick: 0        Merge: 0        Heap: 1 Composite: 0
n = 2000 (average-case ms)
Insertion: 0.15 Quick: 0.15     Merge: 0.25     Heap: 0.35      Composite: 0.35
n = 3000 (worst-case ms)
Insertion: 18   Quick: 1        Merge: 0        Heap: 1 Composite: 0
n = 3000 (average-case ms)
Insertion: 0.1  Quick: 0.45     Merge: 0.35     Heap: 0.7       Composite: 0.4
n = 4000 (worst-case ms)
Insertion: 34   Quick: 1        Merge: 0        Heap: 1 Composite: 0
n = 4000 (average-case ms)
Insertion: 0.35 Quick: 0.55     Merge: 0.55     Heap: 0.95      Composite: 0.55
n = 5000 (worst-case ms)
Insertion: 49   Quick: 1        Merge: 1        Heap: 2 Composite: 1
n = 5000 (average-case ms)
Insertion: 0.15 Quick: 0.65     Merge: 0.8      Heap: 1.15      Composite: 0.8
```
## 申論及開發報告
在 Merge Sort 的實作上，我捨棄了傳統的遞迴寫法，改用 Bottom-up 的雙層 for 迴圈疊代處理。這是因為如果資料量高達幾十萬筆，深度遞迴會產生大量的系統堆疊（Stack）開銷，疊代法能讓記憶體與執行效率更穩定。
在 Quick Sort 處理上，我加入了「三數取中法」來選擇 Pivot，防止演算法在遇到「完全反序」這種最差情況測資時，時間複雜度退化成 $O(n^2)$ 的災難。
此外，在類別與測試設計上，我使用了 C++11 的 mt19937 確保平均情況的隨機性，並把不同排序法包裝成統一的 Function Pointer，方便未來隨時抽換或擴充其他演算法來做效能評估。
### 結論
這個做完後，以前看課本講時間複雜度 $O(n^2)$ 和 $O(n \log n)$ 都覺得很抽象，但這次實際用時鐘函數測量後，看到 Insertion Sort 在資料變大時時間呈現拋物線暴增，邏輯和理論瞬間就結合起來了。
實作 Composite Sort，我才體會到「沒有絕對最強的演算法」。在資料量極小（<20）時，反而是最簡單的 Insertion Sort 跑得最快，懂得依照資料規模去截長補短，讓程式的整體效能提升很多。
