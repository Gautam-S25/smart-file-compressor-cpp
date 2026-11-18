#include "tree_structures.hpp"

BinarySearchTree::BinarySearchTree()
    : root(nullptr) {}

BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

BSTNode* BinarySearchTree::insertRecursive(
    BSTNode* node,
    int key,
    const std::string& value
) {
    if (node == nullptr) {
        return new BSTNode(key, value);
    }

    if (key < node->key) {
        node->left = insertRecursive(
            node->left,
            key,
            value
        );
    }
    else if (key > node->key) {
        node->right = insertRecursive(
            node->right,
            key,
            value
        );
    }
    else {
        node->value = value;
    }

    return node;
}

void BinarySearchTree::insert(
    int key,
    const std::string& value
) {
    root = insertRecursive(
        root,
        key,
        value
    );
}

BSTNode* BinarySearchTree::searchRecursive(
    BSTNode* node,
    int key
) const {
    if (node == nullptr) {
        return nullptr;
    }

    if (key == node->key) {
        return node;
    }

    if (key < node->key) {
        return searchRecursive(
            node->left,
            key
        );
    }

    return searchRecursive(
        node->right,
        key
    );
}

bool BinarySearchTree::search(
    int key,
    std::string& value
) const {
    BSTNode* result =
        searchRecursive(root, key);

    if (result == nullptr) {
        return false;
    }

    value = result->value;

    return true;
}

void BinarySearchTree::inorderRecursive(
    BSTNode* node,
    std::vector<std::pair<int, std::string>>& result
) const {
    if (node == nullptr) {
        return;
    }

    inorderRecursive(
        node->left,
        result
    );

    result.push_back({
        node->key,
        node->value
    });

    inorderRecursive(
        node->right,
        result
    );
}

std::vector<std::pair<int, std::string>>
BinarySearchTree::inorder() const {
    std::vector<std::pair<int, std::string>> result;

    inorderRecursive(
        root,
        result
    );

    return result;
}

void BinarySearchTree::preorderRecursive(
    BSTNode* node,
    std::vector<std::pair<int, std::string>>& result
) const {
    if (node == nullptr) {
        return;
    }

    result.push_back({
        node->key,
        node->value
    });

    preorderRecursive(
        node->left,
        result
    );

    preorderRecursive(
        node->right,
        result
    );
}

std::vector<std::pair<int, std::string>>
BinarySearchTree::preorder() const {
    std::vector<std::pair<int, std::string>> result;

    preorderRecursive(
        root,
        result
    );

    return result;
}

void BinarySearchTree::postorderRecursive(
    BSTNode* node,
    std::vector<std::pair<int, std::string>>& result
) const {
    if (node == nullptr) {
        return;
    }

    postorderRecursive(
        node->left,
        result
    );

    postorderRecursive(
        node->right,
        result
    );

    result.push_back({
        node->key,
        node->value
    });
}

std::vector<std::pair<int, std::string>>
BinarySearchTree::postorder() const {
    std::vector<std::pair<int, std::string>> result;

    postorderRecursive(
        root,
        result
    );

    return result;
}

void BinarySearchTree::destroyTree(
    BSTNode* node
) {
    if (node == nullptr) {
        return;
    }

    destroyTree(
        node->left
    );

    destroyTree(
        node->right
    );

    delete node;
}

bool BinarySearchTree::empty() const {
    return root == nullptr;
}
