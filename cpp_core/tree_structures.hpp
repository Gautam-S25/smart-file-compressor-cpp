#ifndef TREE_STRUCTURES_HPP
#define TREE_STRUCTURES_HPP

#include <iostream>
#include <vector>
#include <string>

struct BSTNode {
    int key;
    std::string value;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int k, const std::string& v)
        : key(k), value(v), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    BSTNode* root;

    BSTNode* insertRecursive(
        BSTNode* node,
        int key,
        const std::string& value
    );

    BSTNode* searchRecursive(
        BSTNode* node,
        int key
    ) const;

    void inorderRecursive(
        BSTNode* node,
        std::vector<std::pair<int, std::string>>& result
    ) const;

    void preorderRecursive(
        BSTNode* node,
        std::vector<std::pair<int, std::string>>& result
    ) const;

    void postorderRecursive(
        BSTNode* node,
        std::vector<std::pair<int, std::string>>& result
    ) const;

    void destroyTree(BSTNode* node);

public:
    BinarySearchTree();

    ~BinarySearchTree();

    void insert(int key, const std::string& value);

    bool search(int key, std::string& value) const;

    std::vector<std::pair<int, std::string>> inorder() const;

    std::vector<std::pair<int, std::string>> preorder() const;

    std::vector<std::pair<int, std::string>> postorder() const;

    bool empty() const;
};

#endif
