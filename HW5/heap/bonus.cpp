#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "heap.h"  // For HEAPSIZE

template <class Node>
struct MinHeap {
   public:
    MinHeap() : _array(new Node[HEAPSIZE]), _size(0) {}
    ~MinHeap() { delete[] _array; }
    void push(const Node &node) {
        // Assume size does not exceed HEAPSIZE
        unsigned pos = _size++;  // Position to put the new element
        for (unsigned parent = (pos - 1) / 2; pos && _array[parent] > node;
             pos = parent, parent = (pos - 1) / 2)
            _array[pos] = _array[parent];  // Swap with parent
        _array[pos] = node;                // Put new Node into array
    }
    void pop() {
        // Assume there exists at least one element in heap
        --_size;
        _array[0] = _array[_size];  // Move last to top
        unsigned pos = 0;           // Position to put the last element
        for (unsigned smallest = 0;;) {
            unsigned left = 2 * pos + 1, right = left + 1;
            if (left < _size && _array[left] < _array[smallest])
                smallest = left;
            if (right < _size && _array[right] < _array[smallest])
                smallest = right;
            if (smallest == pos)
                break;
            swap(pos, smallest);
            pos = smallest;
        }
    }
    unsigned size() const { return _size; }
    bool empty() const { return _size == 0; }
    const Node &top() const { return _array[0]; }

   private:
    void swap(unsigned i, unsigned j) {
        Node temp = _array[i];
        _array[i] = _array[j];
        _array[j] = temp;
    }

   protected:
    Node *_array;
    unsigned _size;
};

struct Counter {
    unsigned _freq;
    const std::string *_word;
    bool operator>(const Counter &other) {
        return _freq != other._freq ? _freq > other._freq
                                    : *_word < *other._word;
    }
    bool operator<(const Counter &other) {
        return _freq != other._freq ? _freq < other._freq
                                    : *_word > *other._word;
    }
};

int main() {
    std::ifstream ifs("bonus.txt");
    std::unordered_map<std::string, unsigned> freq;
    std::string temp;
    unsigned k = 0;

    ifs >> k;  // Get number of words to output
    freq.reserve(k);

    while (ifs && ifs >> temp)  // Count word frequency
        ++freq[std::move(temp)];

    MinHeap<Counter> minHeap;
    for (auto &&i : freq) {  // Add all pair to vector
        minHeap.push(Counter{i.second, &i.first});
        if (minHeap.size() > k)
            minHeap.pop();
    }

    std::vector<const std::string *> reversed;
    reversed.reserve(k);
    while (!minHeap.empty()) {  // Pop top k from heap
        reversed.push_back(minHeap.top()._word);
        minHeap.pop();
    }
    for (auto it = reversed.rbegin(); it != reversed.rend(); ++it)
        std::cout << **it << ' ';
    std::cout.flush();
    return 0;
}