#include "graph_engine.hpp"

#include <stack>

void Graph::addVertex(
    const std::string& vertex
) {
    if (adjacency.find(vertex) == adjacency.end()) {
        adjacency[vertex] = {};
    }
}

void Graph::addEdge(
    const std::string& from,
    const std::string& to
) {
    addVertex(from);
    addVertex(to);

    adjacency[from].push_back(to);
}

std::vector<std::string> Graph::bfs(
    const std::string& start
) const {
    std::vector<std::string> order;

    if (adjacency.find(start) == adjacency.end()) {
        return order;
    }

    std::unordered_set<std::string> visited;
    std::queue<std::string> q;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        order.push_back(current);

        auto it = adjacency.find(current);

        if (it == adjacency.end()) {
            continue;
        }

        for (const std::string& neighbor : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return order;
}

std::vector<std::string> Graph::dfs(
    const std::string& start
) const {
    std::vector<std::string> order;

    if (adjacency.find(start) == adjacency.end()) {
        return order;
    }

    std::unordered_set<std::string> visited;
    std::stack<std::string> st;

    st.push(start);

    while (!st.empty()) {
        std::string current = st.top();
        st.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        visited.insert(current);
        order.push_back(current);

        auto it = adjacency.find(current);

        if (it == adjacency.end()) {
            continue;
        }

        const auto& neighbors = it->second;

        for (auto rit = neighbors.rbegin();
             rit != neighbors.rend();
             ++rit) {

            if (visited.find(*rit) == visited.end()) {
                st.push(*rit);
            }
        }
    }

    return order;
}

bool Graph::dfsCycle(
    const std::string& node,
    std::unordered_set<std::string>& visited,
    std::unordered_set<std::string>& recursionStack
) const {
    visited.insert(node);
    recursionStack.insert(node);

    auto it = adjacency.find(node);

    if (it != adjacency.end()) {
        for (const std::string& neighbor : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                if (dfsCycle(
                        neighbor,
                        visited,
                        recursionStack
                    )) {
                    return true;
                }
            }
            else if (
                recursionStack.find(neighbor)
                != recursionStack.end()
            ) {
                return true;
            }
        }
    }

    recursionStack.erase(node);

    return false;
}

bool Graph::hasCycle() const {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> recursionStack;

    for (const auto& entry : adjacency) {
        const std::string& vertex = entry.first;

        if (visited.find(vertex) == visited.end()) {
            if (dfsCycle(
                    vertex,
                    visited,
                    recursionStack
                )) {
                return true;
            }
        }
    }

    return false;
}

void Graph::display() const {
    std::cout << "\nGraph Adjacency List\n";

    for (const auto& entry : adjacency) {
        std::cout << entry.first << " -> ";

        for (const std::string& neighbor : entry.second) {
            std::cout << neighbor << " ";
        }

        std::cout << "\n";
    }
}
