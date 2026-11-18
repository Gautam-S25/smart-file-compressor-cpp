#!/bin/bash
set -e

echo "Building Nexus Compressor C++ DS Engine..."

g++ -std=c++17 \
cpp_core/main.cpp \
cpp_core/linear_ds.cpp \
cpp_core/tree_structures.cpp \
cpp_core/graph_engine.cpp \
cpp_core/search_sort.cpp \
cpp_core/algorithms.cpp \
cpp_core/huffman_engine.cpp \
-o nexus_engine

echo "Build successful."
echo "Created: $(pwd)/nexus_engine"
