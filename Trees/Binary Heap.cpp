//
// Created by Saeed on 1/4/2023.
//

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

// why the array representation is great?
// b. it tells us the next available index in O(1) !

template <typename type>
class MinHeap {
    type *array { };
    int size { };
    int capacity { 1000 };

    int left(int node) {
        int c = 2*node + 1;
        return c >= size? -1 : c;
    }
    int right(int node) {
        int c = 2*node + 2;
        return c >= size? -1 : c;
    }
    int parent(int node) {
        int p = (node - 1) / 2;
        return node == 0? -1 : p;
    }

    void heapify_up(int childPos) { // O(log n)
        int parPos = parent(childPos);
        // stop if child = root or if reach the correct pos
        if (childPos == 0 or array[parPos] <= array[childPos])
            return;

        swap(array[parPos], array[childPos]);
        heapify_up(parPos);
    }
    void heapify_down(int parentPos) { // O(log n)
        int childPos = left(parentPos);
        int rightChild = right(parentPos);

        if (childPos == -1) // no children
            return;

        // choose one of the children:
        if (rightChild != -1 and array[rightChild] < array[childPos])
            childPos = rightChild;

        if (array[parentPos] > array[childPos]) {
            swap(array[parentPos], array[childPos]);
            heapify_down(childPos);
        }
    }
    // Floyd algo:
    void heapify() { // O(n) NOT O(n logn) => Floyd proved that :) !!
        for(int i = size / 2 - 1; i >= 0; --i) // start from the non-leaf nodes
            heapify_down(i);
    }

public:
    MinHeap() {
        array = new type[capacity] { };
        size = 0;
    }
    MinHeap(const vector<type> &v) {
        // create a heap using Floyd algorithm
        assert(v.size() <= capacity);
        array = new type[capacity] {};
        size = v.size();

        for (int i = 0; i < size; ++i) {
            array[i] = v[i];
        }

        heapify();
    }
    ~MinHeap() {
        delete[] array;
        array = nullptr;
    }

    bool is_empty() { return size == 0; }

    void push(type key) {
        assert(size < capacity);
        array[size++] = key; // add
        heapify_up(size - 1); // fix
    }
    type top() {
        assert(!is_empty());
        return array[0];
    }

    void pop() {
        assert(!is_empty());
        array[0] = array[--size]; // replace with the last element
        heapify_down(0);
    }
};