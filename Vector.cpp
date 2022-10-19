#include <bits/stdc++.h>
using namespace std;

class Vector {
private:
    int* arr = nullptr;
    int size; // filled size
    int capacity; // actual allocated size
public:
    Vector(int size) {
        this->size = size < 0? 1 : size;
        capacity = 2*size;
        arr = new int[capacity] {}; // initialized with 0 by default
    }
    ~Vector() {
        delete[] arr;
        arr = nullptr;
    }

    int get(int idx) {
        assert(idx >= 0 and idx < size); // for exception handling
        return arr[idx];
    }
    void set(int idx, int val) {
        assert(idx >= 0 and idx < size);
        arr[idx] = val;
    }

    void print() {
        for (int i = 0; i < size; ++i) {
            cout << arr[i] << ' ';
        }
        cout << endl;
    }

    int find_idx(int val) {
        for(int i = 0; i < size; ++i) {
            if(arr[i] == val) return i;
        }
        return -1; // not found!
    }

    void init(int val) {
        for (int i = 0; i < size; ++i) {
            arr[i] = val;
        }
    }

    int get_front() {
        assert(size > 0);
        return arr[0];
    }
    int get_back() {
        assert(size > 0);
        return arr[size - 1];
    }

    void expand_capacity() {
        capacity = 2*size;
        int* tmp = new int[capacity];
        for (int i = 0; i < size; ++i) {
            tmp[i] = arr[i];
        }

        swap(arr, tmp);
        delete[] tmp;
    }
    void push_back(int element) {
        if(size == capacity) expand_capacity();

        arr[size] = element;
        size++;
    }

    void insert(int idx, int val) {
        // worst case: O(size)
        assert(idx >= 0 and idx < size);

        if(size == capacity) expand_capacity();
        size++;
        for(int i = size - 1; i > idx; --i)
            arr[i] = arr[i - 1];
        arr[idx] = val;
    }

    /// challenges solutions ///
    void right_rotate() {
        int last = arr[size - 1];
        for (int i = size - 1; i >= 1; --i) {
            arr[i] = arr[i - 1];
        }
        arr[0] = last;
    } // O(size)
    void left_rotate() {
        int first = arr[0];
        for (int i = 0; i < size - 1; ++i) {
            arr[i] = arr[i + 1];
        }
        arr[size - 1] = first;
    } // O(size)

    void right_rotate(int times) {
        int steps = times % size;
        int toShift = size - steps;

        if (steps > 0) {
            int *last_elements = new int[steps];
            for (int i = 0; i < steps; ++i) {
                last_elements[i] = arr[i + toShift];
            }

            // shift elements:
            for (int i = size - 1; i >= steps; --i) {
                arr[i] = arr[i - steps];
            }

            // add last elements:
            for (int i = 0; i < steps; ++i) {
                arr[i] = last_elements[i];
            }
        }
    } // O(size)

    int pop(int idx) {
        assert(idx >= 0 and idx < size);

        int ret = arr[idx];
        // remove:
        for (int i = idx; i < size - 1; ++i) {
            arr[i] = arr[i + 1]; // shift right elements to the left
        }
        size--;

        return ret;
    }

    int find_transposition(int value) {
        for (int i = 0; i < size; ++i) {
            if(arr[i] == value) {
                if(i > 0) swap(arr[i], arr[i - 1]); // shift it to the left
                return max(0, i - 1);
            }
        }

        return -1; // not found
    } // O(size), but with enhanced shifting trick

};

int main()
{
    Vector v(6);
    for (int i = 0; i < 6; ++i) {
        v.set(i, i + 1);
    }
    v.right_rotate(9);
    v.print();

    cout << v.pop(5) << endl;
    v.print();

    cout << v.find_transposition(10) << endl;
    v.print();
    return 0;
}