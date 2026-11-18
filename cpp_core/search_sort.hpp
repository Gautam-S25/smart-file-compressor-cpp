#ifndef SEARCH_SORT_HPP
#define SEARCH_SORT_HPP

#include <vector>
#include <utility>

class SearchSort {
private:
    static int partition(
        std::vector<std::pair<int, long long>>& data,
        int low,
        int high
    );

    static void quickSortRecursive(
        std::vector<std::pair<int, long long>>& data,
        int low,
        int high
    );

public:
    static void quickSort(
        std::vector<std::pair<int, long long>>& data
    );

    static int binarySearch(
        const std::vector<std::pair<int, long long>>& data,
        int key
    );
};

#endif
