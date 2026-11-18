#include "huffman_engine.hpp"
#include "linear_ds.hpp"
#include "algorithms.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <filesystem>

bool HuffmanEngine::compressFile(
    const std::string& inputPath,
    const std::string& outputPath,
    const std::string& originalName
) {
    std::ifstream input(inputPath, std::ios::binary);

    if (!input.is_open()) {
        std::cerr << "Unable to open input file.\n";
        return false;
    }

    std::vector<unsigned char> data(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    input.close();

    if (data.empty()) {
        std::cerr << "Input file is empty.\n";
        return false;
    }

    FrequencyDoublyLinkedList frequencyList;

    for (unsigned char byte : data) {
        frequencyList.insertOrIncrement(byte);
    }

    auto frequencies = frequencyList.toVector();

    HuffmanNode* root =
        Algorithms::buildHuffmanTree(frequencies);

    if (!root) {
        return false;
    }

    auto codes =
        Algorithms::generateHuffmanCodes(root);

    std::string encodedBits;

    for (unsigned char byte : data) {
        encodedBits += codes[byte];
    }

    unsigned char padding =
        static_cast<unsigned char>(
            (8 - encodedBits.size() % 8) % 8
        );

    encodedBits.append(padding, '0');

    std::ofstream output(outputPath, std::ios::binary);

    if (!output.is_open()) {
        Algorithms::deleteHuffmanTree(root);
        return false;
    }

    const char magic[4] = {'N', 'H', 'U', 'F'};
    output.write(magic, 4);

    std::uint16_t nameLength =
        static_cast<std::uint16_t>(originalName.size());

    output.write(
        reinterpret_cast<const char*>(&nameLength),
        sizeof(nameLength)
    );

    output.write(
        originalName.data(),
        nameLength
    );

    std::uint64_t originalSize =
        static_cast<std::uint64_t>(data.size());

    output.write(
        reinterpret_cast<const char*>(&originalSize),
        sizeof(originalSize)
    );

    std::uint16_t uniqueCount =
        static_cast<std::uint16_t>(frequencies.size());

    output.write(
        reinterpret_cast<const char*>(&uniqueCount),
        sizeof(uniqueCount)
    );

    for (const auto& item : frequencies) {
        unsigned char byte = item.first;

        std::uint64_t frequency =
            static_cast<std::uint64_t>(item.second);

        output.write(
            reinterpret_cast<const char*>(&byte),
            sizeof(byte)
        );

        output.write(
            reinterpret_cast<const char*>(&frequency),
            sizeof(frequency)
        );
    }

    output.write(
        reinterpret_cast<const char*>(&padding),
        sizeof(padding)
    );

    for (std::size_t i = 0; i < encodedBits.size(); i += 8) {
        unsigned char compressedByte = 0;

        for (int bit = 0; bit < 8; ++bit) {
            compressedByte <<= 1;

            if (encodedBits[i + bit] == '1') {
                compressedByte |= 1;
            }
        }

        output.write(
            reinterpret_cast<const char*>(&compressedByte),
            sizeof(compressedByte)
        );
    }

    output.close();
    Algorithms::deleteHuffmanTree(root);

    return true;
}

bool HuffmanEngine::decompressFile(
    const std::string& inputPath,
    const std::string& outputDirectory,
    std::string& restoredPath
) {
    std::ifstream input(inputPath, std::ios::binary);

    if (!input.is_open()) {
        std::cerr << "Unable to open compressed file.\n";
        return false;
    }

    char magic[4];
    input.read(magic, 4);

    if (
        magic[0] != 'N' ||
        magic[1] != 'H' ||
        magic[2] != 'U' ||
        magic[3] != 'F'
    ) {
        std::cerr << "Invalid compressed file format.\n";
        return false;
    }

    std::uint16_t nameLength = 0;

    input.read(
        reinterpret_cast<char*>(&nameLength),
        sizeof(nameLength)
    );

    std::string originalName(nameLength, '\0');

    input.read(
        originalName.data(),
        nameLength
    );

    restoredPath =
        (std::filesystem::path(outputDirectory) / originalName).string();

    std::uint64_t originalSize = 0;

    input.read(
        reinterpret_cast<char*>(&originalSize),
        sizeof(originalSize)
    );

    std::uint16_t uniqueCount = 0;

    input.read(
        reinterpret_cast<char*>(&uniqueCount),
        sizeof(uniqueCount)
    );

    std::vector<std::pair<unsigned char, long long>> frequencies;

    for (std::uint16_t i = 0; i < uniqueCount; ++i) {
        unsigned char byte;
        std::uint64_t frequency;

        input.read(
            reinterpret_cast<char*>(&byte),
            sizeof(byte)
        );

        input.read(
            reinterpret_cast<char*>(&frequency),
            sizeof(frequency)
        );

        frequencies.push_back({
            byte,
            static_cast<long long>(frequency)
        });
    }

    unsigned char padding = 0;

    input.read(
        reinterpret_cast<char*>(&padding),
        sizeof(padding)
    );

    std::vector<unsigned char> compressedData(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    input.close();

    HuffmanNode* root =
        Algorithms::buildHuffmanTree(frequencies);

    if (!root) {
        return false;
    }

    std::ofstream output(restoredPath, std::ios::binary);

    if (!output.is_open()) {
        Algorithms::deleteHuffmanTree(root);
        return false;
    }

    if (root->isLeaf()) {
        for (std::uint64_t i = 0; i < originalSize; ++i) {
            output.put(static_cast<char>(root->data));
        }

        output.close();
        Algorithms::deleteHuffmanTree(root);
        return true;
    }

    HuffmanNode* current = root;
    std::uint64_t writtenBytes = 0;

    for (std::size_t i = 0; i < compressedData.size(); ++i) {
        unsigned char byte = compressedData[i];

        int validBits = 8;

        if (
            i == compressedData.size() - 1 &&
            padding > 0
        ) {
            validBits = 8 - padding;
        }

        for (int bit = 7; bit >= 8 - validBits; --bit) {
            bool one =
                (byte & (1 << bit)) != 0;

            current =
                one ? current->right : current->left;

            if (current && current->isLeaf()) {
                output.put(
                    static_cast<char>(current->data)
                );

                ++writtenBytes;

                if (writtenBytes == originalSize) {
                    output.close();
                    Algorithms::deleteHuffmanTree(root);
                    return true;
                }

                current = root;
            }
        }
    }

    output.close();
    Algorithms::deleteHuffmanTree(root);

    return writtenBytes == originalSize;
}
