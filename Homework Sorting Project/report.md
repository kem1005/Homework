# 41343110 and 41343153
# Homework Sorting Project
# Problem
## 解題說明
本題要求實作一套完整的排序效能分析框架，針對插入排序、快速排序、合併排序與堆積排序進行效能比較，並進一步設計一套動態混合排序策略。
在實作架構上，程式透過精確的系統時鐘建立測試基準，並結合亂數生成器來模擬平均情況下的隨機資料分佈。
此外，透過特定邏輯刻意產生反序數列，以精準測試各演算法在最差情況下的時間複雜度表現，藉此驗證其理論效率。
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
| 一 | |Clock accuracy: 0.0001 ms||
| 二 | |n = 500 (worst-case ms)
Insertion: 0    Quick: 0        Merge: 0        Heap: 1 Composite: 0|當資料量為 500 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。| 
| 三 | |n = 500 (average-case ms)
Insertion: 0.05 Quick: 0        Merge: 0.05     Heap: 0.1       Composite: 0.05|當資料量為 500 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 四 | |n = 1000 (worst-case ms)
Insertion: 2    Quick: 0        Merge: 0        Heap: 1 Composite: 0|當資料量為 1000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 五 | |n = 1000 (average-case ms)
Insertion: 0    Quick: 0.1      Merge: 0.1      Heap: 0.2       Composite: 0.2|當資料量為 1000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 六 | |n = 2000 (worst-case ms)
Insertion: 9    Quick: 0        Merge: 0        Heap: 1 Composite: 0|當資料量為 2000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 七 | |n = 2000 (average-case ms)
Insertion: 0.15 Quick: 0.15     Merge: 0.25     Heap: 0.35      Composite: 0.35|當資料量為 2000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 八 | |n = 3000 (worst-case ms)
Insertion: 18   Quick: 1        Merge: 0        Heap: 1 Composite: 0|當資料量為 3000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 九 | |n = 3000 (average-case ms)
Insertion: 0.1  Quick: 0.45     Merge: 0.35     Heap: 0.7       Composite: 0.4|當資料量為 3000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 十 | |n = 4000 (worst-case ms)
Insertion: 34   Quick: 1        Merge: 0        Heap: 1 Composite: 0|當資料量為 4000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 十一 | |n = 4000 (average-case ms)
Insertion: 0.35 Quick: 0.55     Merge: 0.55     Heap: 0.95      Composite: 0.55|當資料量為 4000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
| 十二 | |n = 5000 (worst-case ms)
Insertion: 49   Quick: 1        Merge: 1        Heap: 2 Composite: 1|當資料量為 5000 筆，且資料處於「最差情況」時，各演算法耗費的毫秒數 (ms)。|
| 十三 | |n = 5000 (average-case ms)
Insertion: 0.15 Quick: 0.65     Merge: 0.8      Heap: 1.15      Composite: 0.8|當資料量為 5000 筆，將陣列「隨機打亂 (Random Permutation)」測量 20 次後取平均的耗時。|
   
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
