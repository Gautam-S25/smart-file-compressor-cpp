#include "search_sort.hpp"

#include <algorithm>

int SearchSort::partition(
    std::vector<std::pair<int, long long>>& data,
    int low,
    int high
) {
    long long pivot = data[high].second;

    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (data[j].second <= pivot) {
            ++i;

            std::swap(
                data[i],
                data[j]
            );
        }
    }

    std::swap(
        data[i + 1],
        data[high]
    );

    return i + 1;
}

void SearchSort::quickSortRecursive(
    std::vector<std::pair<int, long long>>& data,
    int low,
    int high
) {
    if (low < high) {
        int pivotIndex =
            partition(
                data,
                low,
                high
            );

        quickSortRecursive(
            data,
            low,
            pivotIndex - 1
        );

        quickSortRecursive(
            data,
            pivotIndex + 1,
            high
        );
    }
}

void SearchSort::quickSort(
    std::vector<std::pair<int, long long>>& data
) {
    if (data.empty()) {
        return;
    }

    quickSortRecursive(
        data,
        0,
        static_cast<int>(data.size()) - 1
    );
}

int SearchSort::binarySearch(
    const std::vector<std::pair<int, long long>>& data,
    int key
) {
    int left = 0;
    int right =
        static_cast<int>(data.size()) - 1;

    while (left <= right) {
        int middle =
            left + (right - left) / 2;

        if (data[middle].first == key) {
            return middle;
        }

        if (data[middle].first < key) {
            left = middle + 1;
        }
        else {
            right = middle - 1;
        }
    }

    return -1;
}
