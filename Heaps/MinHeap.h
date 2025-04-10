#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
class MinHeap {
private:
    std::vector<T> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] >= heap[parent]) break;
            std::swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int left, right, smallest;
        while (true) {
            left = 2 * index + 1;
            right = 2 * index + 2;
            smallest = index;

            if (left < heap.size() && heap[left] < heap[smallest])
                smallest = left;
            if (right < heap.size() && heap[right] < heap[smallest])
                smallest = right;
            if (smallest == index) break;

            std::swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    MinHeap() = default;

    void insert(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    T extractMin() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        T min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return min;
    }

    const T& getMin() const {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap[0];
    }

    bool isEmpty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }
};