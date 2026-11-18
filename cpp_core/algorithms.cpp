#include "algorithms.hpp"

#include <algorithm>

HuffmanNode* Algorithms::buildHuffmanTree(
    const std::vector<std::pair<unsigned char, long long>>& frequencies
) {
    if (frequencies.empty()) {
        return nullptr;
    }

    MinHeap heap;

    for (const auto& item : frequencies) {
        if (item.second > 0) {
            heap.push(
                new HuffmanNode(
                    item.first,
                    item.second
                )
            );
        }
    }

    if (heap.empty()) {
        return nullptr;
    }

    while (heap.size() > 1) {
        HuffmanNode* left = heap.pop();
        HuffmanNode* right = heap.pop();

        long long combinedFrequency =
            left->frequency + right->frequency;

        HuffmanNode* parent =
            new HuffmanNode(
                combinedFrequency,
                left,
                right
            );

        heap.push(parent);
    }

    return heap.pop();
}

void Algorithms::generateCodesRecursive(
    HuffmanNode* node,
    const std::string& code,
    std::unordered_map<unsigned char, std::string>& codes
) {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf()) {
        if (code.empty()) {
            codes[node->data] = "0";
        }
        else {
            codes[node->data] = code;
        }

        return;
    }

    generateCodesRecursive(
        node->left,
        code + "0",
        codes
    );

    generateCodesRecursive(
        node->right,
        code + "1",
        codes
    );
}

std::unordered_map<unsigned char, std::string>
Algorithms::generateHuffmanCodes(
    HuffmanNode* root
) {
    std::unordered_map<unsigned char, std::string> codes;

    generateCodesRecursive(
        root,
        "",
        codes
    );

    return codes;
}

bool Algorithms::validatePrefixFree(
    const std::unordered_map<unsigned char, std::string>& codes
) {
    std::vector<std::string> codeList;

    for (const auto& entry : codes) {
        codeList.push_back(entry.second);
    }

    for (std::size_t i = 0; i < codeList.size(); ++i) {
        for (std::size_t j = 0; j < codeList.size(); ++j) {
            if (i == j) {
                continue;
            }

            const std::string& a = codeList[i];
            const std::string& b = codeList[j];

            if (
                b.size() >= a.size() &&
                b.compare(0, a.size(), a) == 0
            ) {
                return false;
            }
        }
    }

    return true;
}

int Algorithms::knapsack(
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity
) {
    int n = static_cast<int>(weights.size());

    if (
        n == 0 ||
        values.size() != weights.size() ||
        capacity <= 0
    ) {
        return 0;
    }

    std::vector<std::vector<int>> dp(
        n + 1,
        std::vector<int>(capacity + 1, 0)
    );

    for (int i = 1; i <= n; ++i) {
        for (int c = 0; c <= capacity; ++c) {
            dp[i][c] = dp[i - 1][c];

            if (weights[i - 1] <= c) {
                dp[i][c] = std::max(
                    dp[i][c],
                    values[i - 1] +
                    dp[i - 1][c - weights[i - 1]]
                );
            }
        }
    }

    return dp[n][capacity];
}

void Algorithms::deleteHuffmanTree(
    HuffmanNode* root
) {
    if (root == nullptr) {
        return;
    }

    deleteHuffmanTree(root->left);
    deleteHuffmanTree(root->right);

    delete root;
}
