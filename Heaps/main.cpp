#include <iostream>
#include "MaxHeap.h"
#include "MinHeap.h"

std::vector<int> findKSmallest(const std::vector<int>& mass, int k) {
    if (k <= 0 || k > mass.size()) {
        return {};
    }

    MaxHeap<int> heap;

    // Добавляем первые k элементов
    for (int i = 0; i < k; ++i) {
        heap.insert(mass[i]);
    }

    // Обрабатываем остальные элементы
    for (int i = k; i < mass.size(); ++i) {
        if (mass[i] < heap.getMax()) {
            heap.extractMax();
            heap.insert(mass[i]);
        }
    }

    // Извлекаем результаты из кучи
    std::vector<int> result;
    while (!heap.isEmpty()) {
        result.push_back(heap.extractMax());
    }

    // Так как это max-heap, элементы извлекаются от большего к меньшему, поэтому разворачиваем результат
    std::reverse(result.begin(), result.end());

    return result;
}

std::vector<int> findKLargest(const std::vector<int>& mass, int k) {
    if (k <= 0 || k > mass.size()) {
        return {};
    }

    MinHeap<int> heap;

    // Добавляем первые k элементов
    for (int i = 0; i < k; ++i) {
        heap.insert(mass[i]);
    }

    // Обрабатываем остальные элементы
    for (int i = k; i < mass.size(); ++i) {
        if (mass[i] > heap.getMin()) {
            heap.extractMin();
            heap.insert(mass[i]);
        }
    }

    // Извлекаем результаты из кучи
    std::vector<int> result;
    while (!heap.isEmpty()) {
        result.push_back(heap.extractMin());
    }

    // Так как это min-heap, элементы извлекаются от меньшего к большему, поэтому разворачиваем результат
    std::reverse(result.begin(), result.end());

    return result;
}

int main() {
    std::vector<int> mass = { 4, 1, 3, 12, 7, 14, 9, 10, 2, 5, 8, 6, 11, 13 };
    int k = 5;

    auto kSmallest = findKSmallest(mass, k);
    std::cout << k << " smallest elements: ";
    for (int num : kSmallest) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    auto kLargest = findKLargest(mass, k);
    std::cout << k << " largest elements: ";
    for (int num : kLargest) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}