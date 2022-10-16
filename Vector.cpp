#include <bits/stdc++.h>
using namespace std;

class Vector {
private:
    int* arr = nullptr;
    int size;
public:
    Vector(int size) {
        this->size = size < 0? 1 : size;
        arr = new int[size] {}; // initialized with 0 by default
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

    void Push_back(int element) {
        arr[size] = element;
        size++;
    }

    void insert(int idx, int val) {
        // worst case: O(size)
        assert(idx >= 0 and idx < size);
        size++;
        for(int i = size - 1; i > idx; --i)
            arr[i] = arr[i - 1];
        arr[idx] = val;
    }
};

int main()
{
    Vector vec(10);
    vec.print();

    return 0;
}