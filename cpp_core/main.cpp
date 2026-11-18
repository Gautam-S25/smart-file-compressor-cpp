#include "huffman_engine.hpp"
#include "graph_engine.hpp"
#include "search_sort.hpp"
#include "tree_structures.hpp"
#include "algorithms.hpp"
#include "linear_ds.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>

void printUsage() {
    std::cout << "\nNexus Compressor C++ DS Engine\n\n";
    std::cout << "Usage:\n";
    std::cout << "  ./nexus_engine compress <input> <output>\n";
    std::cout << "  ./nexus_engine decompress <input> <output>\n";
    std::cout << "  ./nexus_engine analyze <input>\n";
    std::cout << "  ./nexus_engine demo\n\n";
}

void runDSDemo() {
    std::cout << "\n===== C++ DATA STRUCTURES DEMO =====\n";

    FrequencyDoublyLinkedList list;
    std::string text = "DATASTRUCTURES";

    for (unsigned char c : text) {
        list.insertOrIncrement(c);
    }

    std::cout << "\n1. Doubly Linked List\n";
    list.display();

    LinkedStack stack;
    stack.push("Compression");
    stack.push("Huffman Tree");
    stack.push("Min Heap");

    std::cout << "\n2. Stack\n";

    while (!stack.empty()) {
        std::cout << stack.pop() << "\n";
    }

    BinarySearchTree bst;
    bst.insert(50, "Root");
    bst.insert(30, "Left");
    bst.insert(70, "Right");
    bst.insert(20, "Left Left");
    bst.insert(40, "Left Right");

    std::cout << "\n3. Binary Search Tree\n";

    for (const auto& item : bst.inorder()) {
        std::cout << item.first << " -> " << item.second << "\n";
    }

    Graph graph;
    graph.addEdge("Upload", "Frequency");
    graph.addEdge("Frequency", "Heap");
    graph.addEdge("Heap", "HuffmanTree");
    graph.addEdge("HuffmanTree", "Encode");
    graph.addEdge("Encode", "Output");

    std::cout << "\n4. BFS\n";
    for (const auto& node : graph.bfs("Upload")) {
        std::cout << node << " ";
    }

    std::cout << "\n\n5. DFS\n";
    for (const auto& node : graph.dfs("Upload")) {
        std::cout << node << " ";
    }

    std::cout << "\n";

    std::vector<std::pair<int, long long>> values = {
        {65, 10},
        {66, 4},
        {67, 7},
        {68, 2},
        {69, 12}
    };

    SearchSort::quickSort(values);

    std::cout << "\n6. Quick Sort\n";

    for (const auto& item : values) {
        std::cout
            << static_cast<char>(item.first)
            << " : "
            << item.second
            << "\n";
    }

    std::sort(
        values.begin(),
        values.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    );

    int index = SearchSort::binarySearch(values, 67);

    std::cout << "\n7. Binary Search\n";

    if (index != -1) {
        std::cout << "Character C found\n";
    } else {
        std::cout << "Character not found\n";
    }

    std::vector<int> weights = {1, 3, 4, 5};
    std::vector<int> valuesDP = {1, 4, 5, 7};

    int result = Algorithms::knapsack(
        weights,
        valuesDP,
        7
    );

    std::cout << "\n8. Dynamic Programming\n";
    std::cout << "Knapsack result: " << result << "\n";

    std::cout << "\n===== DEMO COMPLETE =====\n";
}

int analyzeFile(const std::string& inputPath) {
    std::ifstream input(inputPath, std::ios::binary);

    if (!input.is_open()) {
        std::cerr << "Unable to open input file.\n";
        return 1;
    }

    std::vector<unsigned char> data(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    input.close();

    if (data.empty()) {
        std::cerr << "Input file is empty.\n";
        return 1;
    }

    FrequencyDoublyLinkedList frequencyList;

    for (unsigned char byte : data) {
        frequencyList.insertOrIncrement(byte);
    }

    auto frequencies = frequencyList.toVector();

    HuffmanNode* root =
        Algorithms::buildHuffmanTree(frequencies);

    auto codes =
        Algorithms::generateHuffmanCodes(root);

    long long totalEncodedBits = 0;

    for (const auto& item : frequencies) {
        totalEncodedBits +=
            item.second *
            static_cast<long long>(
                codes[item.first].size()
            );
    }

    double averageCodeLength =
        static_cast<double>(totalEncodedBits) /
        static_cast<double>(data.size());

    std::sort(
        frequencies.begin(),
        frequencies.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        }
    );

    std::cout << "FILE_SIZE=" << data.size() << "\n";
    std::cout << "UNIQUE_BYTES=" << frequencies.size() << "\n";
    std::cout << "TOTAL_ENCODED_BITS=" << totalEncodedBits << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "AVG_CODE_LENGTH=" << averageCodeLength << "\n";

    std::cout << "TOP_FREQUENCIES=";

    int limit =
        std::min(
            static_cast<int>(frequencies.size()),
            10
        );

    for (int i = 0; i < limit; ++i) {
        if (i > 0) {
            std::cout << ",";
        }

        std::cout
            << static_cast<int>(frequencies[i].first)
            << ":"
            << frequencies[i].second;
    }

    std::cout << "\n";

    std::cout << "CODEBOOK=";

    bool firstCode = true;

    for (const auto& entry : codes) {
        if (!firstCode) {
            std::cout << ",";
        }

        firstCode = false;

        std::cout
            << static_cast<int>(entry.first)
            << ":"
            << entry.second;
    }

    std::cout << "\n";


    std::cout << "CODEBOOK_DETAILS=";

    bool firstDetail = true;

    for (const auto& item : frequencies) {
        auto codeIt = codes.find(item.first);

        if (codeIt == codes.end()) {
            continue;
        }

        if (!firstDetail) {
            std::cout << ";";
        }

        firstDetail = false;

        std::cout
            << static_cast<int>(item.first)
            << ":"
            << item.second
            << ":"
            << codeIt->second;
    }

    std::cout << "\n";

    Algorithms::deleteHuffmanTree(root);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "demo") {
        runDSDemo();
        return 0;
    }

    if (command == "analyze") {
        if (argc != 3) {
            printUsage();
            return 1;
        }

        return analyzeFile(argv[2]);
    }

    if (command == "compress") {
        if (argc != 4) {
            printUsage();
            return 1;
        }

        bool success =
            HuffmanEngine::compressFile(
                argv[2],
                argv[3],
                std::filesystem::path(argv[2]).filename().string()
            );

        if (success) {
            std::cout << "Compression successful.\n";
            return 0;
        }

        std::cerr << "Compression failed.\n";
        return 1;
    }

    if (command == "decompress") {
        if (argc != 4) {
            printUsage();
            return 1;
        }

        std::string restoredPath;

        bool success =
            HuffmanEngine::decompressFile(
                argv[2],
                argv[3],
                restoredPath
            );

        if (success) {
            std::cout << "Decompression successful.\n";
            std::cout << "RESTORED_PATH=" << restoredPath << "\n";
            return 0;
        }

        std::cerr << "Decompression failed.\n";
        return 1;
    }

    printUsage();
    return 1;
}
