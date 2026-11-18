#ifndef GRAPH_ENGINE_HPP
#define GRAPH_ENGINE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjacency;

    bool dfsCycle(
        const std::string& node,
        std::unordered_set<std::string>& visited,
        std::unordered_set<std::string>& recursionStack
    ) const;

public:
    void addVertex(const std::string& vertex);

    void addEdge(
        const std::string& from,
        const std::string& to
    );

    std::vector<std::string> bfs(
        const std::string& start
    ) const;

    std::vector<std::string> dfs(
        const std::string& start
    ) const;

    bool hasCycle() const;

    void display() const;
};

#endif
