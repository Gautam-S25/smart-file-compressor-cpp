#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "linear_ds.hpp"

#include <unordered_map>
#include <vector>
#include <string>

class Algorithms {
private:
    static void generateCodesRecursive(
        HuffmanNode* node,
        const std::string& code,
        std::unordered_map<unsigned char, std::string>& codes
    );

public:
    static HuffmanNode* buildHuffmanTree(
        const std::vector<std::pair<unsigned char, long long>>& frequencies
    );

    static std::unordered_map<unsigned char, std::string>
    generateHuffmanCodes(
        HuffmanNode* root
    );

    static bool validatePrefixFree(
        const std::unordered_map<unsigned char, std::string>& codes
    );

    static int knapsack(
        const std::vector<int>& weights,
        const std::vector<int>& values,
        int capacity
    );

    static void deleteHuffmanTree(
        HuffmanNode* root
    );
};

#endif
