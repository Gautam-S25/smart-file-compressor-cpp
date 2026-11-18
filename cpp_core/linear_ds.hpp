#ifndef LINEAR_DS_HPP
#define LINEAR_DS_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

struct HuffmanNode {
    unsigned char data;
    long long frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char d, long long f)
        : data(d), frequency(f), left(nullptr), right(nullptr) {}

    HuffmanNode(long long f, HuffmanNode* l, HuffmanNode* r)
        : data(0), frequency(f), left(l), right(r) {}

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};


// ============================================================
// DOUBLY LINKED LIST
// ============================================================

struct FrequencyNode {
    unsigned char data;
    long long frequency;

    FrequencyNode* prev;
    FrequencyNode* next;

    FrequencyNode(unsigned char d)
        : data(d), frequency(1), prev(nullptr), next(nullptr) {}
};


class FrequencyDoublyLinkedList {

private:
    FrequencyNode* head;
    FrequencyNode* tail;

public:

    FrequencyDoublyLinkedList();

    ~FrequencyDoublyLinkedList();

    void insertOrIncrement(unsigned char data);

    long long getFrequency(unsigned char data) const;

    std::vector<std::pair<unsigned char,long long>> toVector() const;

    void display() const;
};


// ============================================================
// STACK
// ============================================================

class LinkedStack {

private:

    struct StackNode {

        std::string data;
        StackNode* next;

        StackNode(const std::string& value)
            : data(value), next(nullptr) {}
    };

    StackNode* topNode;

public:

    LinkedStack();

    ~LinkedStack();

    void push(const std::string& value);

    std::string pop();

    std::string top() const;

    bool empty() const;
};


// ============================================================
// CUSTOM MIN HEAP
// ============================================================

class MinHeap {

private:

    std::vector<HuffmanNode*> heap;

    void heapifyUp(int index);

    void heapifyDown(int index);

public:

    void push(HuffmanNode* node);

    HuffmanNode* pop();

    HuffmanNode* top() const;

    bool empty() const;

    int size() const;
};

#endif
