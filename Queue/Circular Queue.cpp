/// Circular Queue
// 17 / 12 / 2022
// author: Saeed Ahmed

#include <iostream>
#include <assert.h>
using namespace std;

template <typename type>
class Queue {
    int size {};
    int front {0}, rear {0};
    int added_elements {};
    type *array {};

    /// Utility Functions:

    int next(int pos) {
        // return (pos + 1) % size

        ++pos;
        if (pos == size)
            pos = 0;
        return pos;

        // note: "if" condition is faster than the division and mod
        // so it's preferred in the industry
    }

public:
    Queue(int size) : size(size) {
        array = new type[size];
    }

    bool isEmpty() { return added_elements == 0; }
    bool isFull() { return added_elements == size; }

    void enqueue(int value) {
        assert(!isFull());
        array[rear] = value;
        rear = next(rear);
        ++added_elements;
    }
    int dequeue() {
        assert(!isEmpty());
        type ret = array[front];
        front = next(front);
        --added_elements;
        return ret;
    }

    void display() {
        cout << "Front: " << front << " - Rear: " << rear << '\t';
        if (isFull())
            cout << "full";
        else if (isEmpty()) {
            cout << "empty";
            return;
        }
        cout << '\n';

        for (int cur = front, step = 0; step < added_elements;
            cur = next(cur), ++step)
            cout << array[cur] << ' ';
        cout << "\n\n";
    }

};


//////////// test & main ////////////

void test() {
    Queue<int> queue(10);
    for (int i = 1; i <= 7; ++i) {
        queue.enqueue(i);
    }
    queue.display();

    while (!queue.isEmpty())
        cout << queue.dequeue() << ' ';
}

int main() {
    test(); return 0;
}