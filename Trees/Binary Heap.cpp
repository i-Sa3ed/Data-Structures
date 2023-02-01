//
// Created by Saeed on 1/4/2023.
//

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

// why the array representation is great?
// b. it tells us the next available index in O(1) !

// Notice: Heap must be a complete tree

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

    // Floyd algo:
    void heapify() { // O(n) NOT O(n logn) => Floyd proved that :) !!
        for(int i = size / 2 - 1; i >= 0; --i) // skip all leaf nodes, first non-leaf node pos = (size / 2 - 1)
            heapify_down(i);
    }

    void heapify_down(int parentPos) { // O(log n)
        int childPos = left(parentPos);
        int rightChild = right(parentPos);

        if (childPos == -1) // no children
            return;

        // choose one of the children (min of them)
        if (rightChild != -1 and array[rightChild] < array[childPos])
            childPos = rightChild;

        if (array[parentPos] > array[childPos]) {
            swap(array[parentPos], array[childPos]);
            heapify_down(childPos);
        }
    }

public:
    MinHeap() {
        array = new type[capacity] { };
        size = 0;
    }
    MinHeap(const vector<type> &v) {
        // create a heap using Floyd algorithm
        assert(v.size() <= capacity);
        array = new type[capacity] { };
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
        array[size] = key; // add
        heapify_up(size); // fix
        ++size;
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

    /// Easy to Medium challenges
    void print_less_than(type val, int parentPos = 0) {
        if (is_empty() or parentPos == -1 or array[parentPos] >= val) // stop if: reach a greater value, or invalid index
            return;

        cout << array[parentPos] << ' ';
        print_less_than(val, left(parentPos));
        print_less_than(val, right(parentPos));
    }

    bool is_heap_array(type* p, int n, int parentPos = 0) {
        // pass array 'p' => don't corrupt my original array

        if (parentPos >= n)
            return true;

        type mn = INT_MAX;
        int l = 2*parentPos + 1, r = l + 1;
        if (l < n)
            mn = min(mn, p[l]);
        if (r < n)
            mn = min(mn, p[l]);

        bool res = p[parentPos] <= mn;
        return res and is_heap_array(p, n, l) and is_heap_array(p, n, r);

        // to use class functions => copy the data to 'array' , use it , then replace again
    }

    void heap_sort(type* p, int n) { // O(n logn)
        type* oldArr = array;
        int oldSize = size;

        array = p, size = n; // copy
        heapify(); // O(n)

        while (size) {
            --size;
            // dequeue first element => push it to the end of the array
            // this is the trick of sorting in-place
            swap(array[0], array[size]);
            heapify_down(0);
        }

        // now the array is sorted, but descending, let's reverse it
        for (int i = 0; i < n / 2; ++i) {
            swap(array[i], array[n-i-1]);
        }

        array = oldArr, size = oldSize; // back to original
    }
};

//////////////////////////////////////////////

/// Max Heap
// just reverse the conditions of heapifying functions
template <typename type>
class MaxHeap {
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
        if (childPos == 0 or array[parPos] >= array[childPos])
            return;

        swap(array[parPos], array[childPos]);
        heapify_up(parPos);
    }
    void heapify_down(int parentPos) { // O(log n)
        int childPos = left(parentPos);
        int rightChild = right(parentPos);

        if (childPos == -1) // no children
            return;

        // choose one of the children (max of them)
        if (rightChild != -1 and array[rightChild] > array[childPos])
            childPos = rightChild;

        if (array[parentPos] < array[childPos]) {
            swap(array[parentPos], array[childPos]);
            heapify_down(childPos);
        }
    }
    // Floyd algo:
    void heapify() { // O(n) NOT O(n logn) => Floyd proved that :) !!
        for(int i = size / 2 - 1; i >= 0; --i) { // skip all leaf nodes, first non-leaf node pos = (size / 2 - 1)
            heapify_down(i);
        }
    }

public:
    MaxHeap() {
        array = new type[capacity] { };
        size = 0;
    }
    MaxHeap(const vector<type> &v) {
        // create a heap using Floyd algorithm
        assert(v.size() <= capacity);
        array = new type[capacity] {};
        size = v.size();

        for (int i = 0; i < size; ++i) {
            array[i] = v[i];
        }

        heapify();
    }
    ~MaxHeap() {
        delete[] array;
        array = nullptr;
    }

    bool is_empty() { return size == 0; }

    void push(type key) {
        assert(size < capacity);
        array[size] = key; // add
        heapify_up(size); // fix
        ++size;
    }
    type top() {
        assert(!is_empty());
        return array[0];
    }

    void pop() { // O(log n)
        assert(!is_empty());
        array[0] = array[--size]; // replace with the last element
        heapify_down(0);
    }
    void clear() {
        size = 0;
    }
};


/////////////////////////////////////////////////

/// Challenge: Max heap using Min heap
// idea: in push: multiply by -1 => acting as max heap
// then in top: multiply bach by -1
template <typename type>
class MaxHeapAgg {
    MinHeap<type> heap;
    vector<type> negate(const vector<type>& v) {
        vector<type> res = v;
        int n = res.size();
        for (int i = 0; i < n; ++i) {
            res[i] *= -1;
        }
        return res;
    }

public:
    MaxHeapAgg() {
    }
    MaxHeapAgg(const vector<type>& v) {
        heap = MinHeap<type>(negate(v));
    }

    bool is_empty() { return heap.is_empty(); }

    void push(type val) {
        heap.push(-val);
    }
    type top() {
        return -heap.top();
    }
    void pop() {
        heap.pop();
    }
};

//////////////////////////////////////////////

/// test ///

void print_and_clear(MaxHeap<int>& heap) {
    while (!heap.is_empty())
        cout << heap.top() << ' ', heap.pop();
    cout << endl;
}
void print_and_clear(MinHeap<int>& h) {
    while (!h.is_empty())
        cout << h.top() << ' ', h.pop();
    cout << endl;
}
void print_and_clear(MaxHeapAgg<int>& heap) {
    while (!heap.is_empty())
        cout << heap.top() << ' ', heap.pop();
    cout << endl;
}

void test_min_heap() {
    vector<int> v { 2, 17, 22, 10, 8, 14, 19, 7, 6, 5, 12 };
    MinHeap<int> heap(v);

    int n = 14;
    int arr1[] = { 2, 17, 22, 10, 8, 37, 14, 19, 7, 6, 5, 12, 25, 30 };
    int arr2[] = {2, 5, 12, 7,6, 22, 14, 19, 10, 17, 8, 37, 25, 30};
    //cout << heap.is_heap_array(arr1, n) << endl; // false
    cout << heap.is_heap_array(arr2, n) << endl; // true

    // heap sort:
    heap.heap_sort(arr1, n);
    for (int i = 0; i < n; ++i) {
        cout << arr1[i] << ' ';
    }
    cout << endl;

    print_and_clear(heap);


    //heap.print_less_than(10); cout << endl;

    //print_and_clear(heap);

}

void test_max_heap() {
    MaxHeap<int> heap;
    heap.push(10);
    heap.push(1);
    heap.push(9);
    heap.push(17);
    heap.push(15);
    heap.push(6);

    print_and_clear(heap);

    heap = MaxHeap<int>({10, 9, 15, 17, 1, 6});

    print_and_clear(heap);
}

void test_max_heap_aggregated() {
    vector<int> v = {8, 234, 388, 1, 23, 0};
    MaxHeapAgg<int> maxHeapAgg(v);
    print_and_clear(maxHeapAgg);
}

int main() {
    test_min_heap();
    //test_max_heap();
    //test_max_heap_aggregated();


    cout << "\n bye :)\n";
    return 0;
}