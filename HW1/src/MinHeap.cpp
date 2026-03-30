#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

template <class T>
class IMinPriorityQueue {
public:
    virtual ~IMinPriorityQueue() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& item) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public IMinPriorityQueue<T> {
private:
    T* elements;
    int maxCapacity;
    int currentCount;

    void ExpandCapacity() {
        maxCapacity *= 2;
        T* newElements = new T[maxCapacity + 1];
        
        for (int i = 1; i <= currentCount; i++) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
    }

public:
    MinHeap(int initialCapacity = 10) {
        maxCapacity = initialCapacity;
        elements = new T[maxCapacity + 1]; 
        currentCount = 0;
    }

    ~MinHeap() {
        delete[] elements;
    }

    bool IsEmpty() const override {
        return currentCount == 0;
    }

    const T& Top() const override {
        if (IsEmpty()) {
            throw runtime_error("目前 Heap 是空的，無法取得頂端元素！");
        }
        return elements[1];
    }

    void Push(const T& newItem) override {
        if (currentCount + 1 == maxCapacity) {
            ExpandCapacity();
        }

        int currentIndex = ++currentCount;

        while (currentIndex != 1 && newItem < elements[currentIndex / 2]) {
            elements[currentIndex] = elements[currentIndex / 2];
            currentIndex /= 2;
        }

        elements[currentIndex] = newItem;
    }

    void Pop() override {
        if (IsEmpty()) {
            throw runtime_error("目前 Heap 已經空了，無法進行刪除喔！");
        }

        T lastElement = elements[currentCount--];

        int parentIndex = 1;
        int childIndex = 2;

        while (childIndex <= currentCount) {
            if (childIndex < currentCount && elements[childIndex] > elements[childIndex + 1]) {
                childIndex++;
            }

            if (lastElement <= elements[childIndex]) {
                break;
            }

            elements[parentIndex] = elements[childIndex];
            parentIndex = childIndex;
            childIndex *= 2;
        }

        elements[parentIndex] = lastElement;
    }

    void PrintByIndex() const {
        if (IsEmpty()) {
            cout << "Heap 目前沒有元素。" << endl;
            return;
        }
        
        for (int i = 1; i <= currentCount; i++) {
            cout << elements[i] << (i == currentCount ? "" : " -> ");
        }
        cout << endl;
    }
};

int main() {
    MinHeap<int> myMinHeap;
    int totalItems, inputValue;

    cout << "test Min-Heap" << endl;
    cout << "請輸入n個數字：";
    cin >> totalItems;

    cout << "請依序輸入 " << totalItems << " 個元素 ：\n> ";
    for (int i = 0; i < totalItems; i++) {
        cin >> inputValue;
        myMinHeap.Push(inputValue);
    }

    cout << "\n完成！目前 Heap 的內部陣列結構 (依 index 順序)：\n";
    myMinHeap.PrintByIndex();
    cout << "=======================================" << endl;

    return 0;
}
