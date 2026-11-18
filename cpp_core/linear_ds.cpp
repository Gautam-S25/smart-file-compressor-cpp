#include "linear_ds.hpp"

#include <stdexcept>
#include <algorithm>


// ============================================================
// DOUBLY LINKED LIST
// ============================================================

FrequencyDoublyLinkedList::FrequencyDoublyLinkedList()
    : head(nullptr), tail(nullptr) {}


FrequencyDoublyLinkedList::~FrequencyDoublyLinkedList() {

    FrequencyNode* current = head;

    while (current != nullptr) {

        FrequencyNode* nextNode = current->next;

        delete current;

        current = nextNode;
    }
}


void FrequencyDoublyLinkedList::insertOrIncrement(unsigned char data) {

    FrequencyNode* current = head;

    while (current != nullptr) {

        if (current->data == data) {

            current->frequency++;

            return;
        }

        current = current->next;
    }


    FrequencyNode* newNode = new FrequencyNode(data);

    if (head == nullptr) {

        head = newNode;
        tail = newNode;

        return;
    }


    tail->next = newNode;

    newNode->prev = tail;

    tail = newNode;
}


long long FrequencyDoublyLinkedList::getFrequency(
    unsigned char data
) const {

    FrequencyNode* current = head;

    while (current != nullptr) {

        if (current->data == data) {

            return current->frequency;
        }

        current = current->next;
    }

    return 0;
}


std::vector<std::pair<unsigned char,long long>>
FrequencyDoublyLinkedList::toVector() const {

    std::vector<std::pair<unsigned char,long long>> result;

    FrequencyNode* current = head;

    while (current != nullptr) {

        result.push_back(
            {current->data, current->frequency}
        );

        current = current->next;
    }

    return result;
}


void FrequencyDoublyLinkedList::display() const {

    FrequencyNode* current = head;

    std::cout << "\nFrequency Doubly Linked List\n";

    while (current != nullptr) {

        std::cout
            << static_cast<int>(current->data)
            << " : "
            << current->frequency
            << "\n";

        current = current->next;
    }
}



// ============================================================
// STACK
// ============================================================

LinkedStack::LinkedStack()
    : topNode(nullptr) {}


LinkedStack::~LinkedStack() {

    while (!empty()) {

        pop();
    }
}


void LinkedStack::push(const std::string& value) {

    StackNode* newNode =
        new StackNode(value);

    newNode->next = topNode;

    topNode = newNode;
}


std::string LinkedStack::pop() {

    if (empty()) {

        throw std::runtime_error(
            "Stack underflow"
        );
    }


    StackNode* temp = topNode;

    std::string value = temp->data;

    topNode = topNode->next;

    delete temp;

    return value;
}


std::string LinkedStack::top() const {

    if (empty()) {

        throw std::runtime_error(
            "Stack is empty"
        );
    }

    return topNode->data;
}


bool LinkedStack::empty() const {

    return topNode == nullptr;
}



// ============================================================
// CUSTOM MIN HEAP
// ============================================================

void MinHeap::heapifyUp(int index) {

    while (index > 0) {

        int parent = (index - 1) / 2;

        if (
            heap[parent]->frequency
            <= heap[index]->frequency
        ) {

            break;
        }


        std::swap(
            heap[parent],
            heap[index]
        );

        index = parent;
    }
}


void MinHeap::heapifyDown(int index) {

    int n = heap.size();

    while (true) {

        int smallest = index;

        int left =
            2 * index + 1;

        int right =
            2 * index + 2;


        if (
            left < n &&
            heap[left]->frequency
            < heap[smallest]->frequency
        ) {

            smallest = left;
        }


        if (
            right < n &&
            heap[right]->frequency
            < heap[smallest]->frequency
        ) {

            smallest = right;
        }


        if (smallest == index) {

            break;
        }


        std::swap(
            heap[index],
            heap[smallest]
        );

        index = smallest;
    }
}


void MinHeap::push(
    HuffmanNode* node
) {

    heap.push_back(node);

    heapifyUp(
        heap.size() - 1
    );
}


HuffmanNode* MinHeap::pop() {

    if (heap.empty()) {

        throw std::runtime_error(
            "Heap is empty"
        );
    }


    HuffmanNode* minimum =
        heap.front();


    heap[0] =
        heap.back();


    heap.pop_back();


    if (!heap.empty()) {

        heapifyDown(0);
    }


    return minimum;
}


HuffmanNode* MinHeap::top() const {

    if (heap.empty()) {

        return nullptr;
    }

    return heap.front();
}


bool MinHeap::empty() const {

    return heap.empty();
}


int MinHeap::size() const {

    return static_cast<int>(
        heap.size()
    );
}
