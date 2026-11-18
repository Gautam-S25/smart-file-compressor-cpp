#ifndef HUFFMAN_ENGINE_HPP
#define HUFFMAN_ENGINE_HPP

#include <string>

class HuffmanEngine {
public:
    static bool compressFile(
        const std::string& inputPath,
        const std::string& outputPath,
        const std::string& originalName
    );

    static bool decompressFile(
        const std::string& inputPath,
        const std::string& outputDirectory,
        std::string& restoredPath
    );
};

#endif
