#include "quicksort.h"
#include <algorithm>
#include <utility>

// https://en.wikipedia.org/wiki/Quicksort

static int medianOfThree(const std::vector<int>& arr, int low, int high) {
    int middle = low + (high - low) / 2;
    if (arr[low] > arr[middle]) std::swap(low, middle);
    if (arr[low] > arr[high]) std::swap(low, high);
    if (arr[middle] > arr[high]) std::swap(middle, high);
    return middle;
}

static std::pair<int, int> partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[medianOfThree(arr, low, high)];
    int less = low;
    int current = low;
    int greater = high;

    while (current <= greater) {
        if (arr[current] < pivot) {
            std::swap(arr[less++], arr[current++]);
        } else if (arr[current] > pivot) {
            std::swap(arr[current], arr[greater--]);
        } else {
            ++current;
        }
    }
    return {less, greater};
}

static void quickSortRec(std::vector<int>& arr, int low, int high) {
    while (low < high) {
        auto [equalStart, equalEnd] = partition(arr, low, high);

        if (equalStart - low < high - equalEnd) {
            quickSortRec(arr, low, equalStart - 1);
            low = equalEnd + 1;
        } else {
            quickSortRec(arr, equalEnd + 1, high);
            high = equalStart - 1;
        }
    }
}

void quickSort(std::vector<int>& arr) {
    if (arr.size() < 2) return;
    quickSortRec(arr, 0, static_cast<int>(arr.size()) - 1);
}