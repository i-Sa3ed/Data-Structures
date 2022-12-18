//
// Created by Saeed Ahmed on 12/17/2022.
//

/// Circular Deque
// 17 / 12 / 2022
// author: Saeed Ahmed

#include <iostream>
#include <assert.h>
using namespace std;

////////// deque /////////

template <typename type>
class Deque { // double ended queue
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
    int prev (int pos) {
        // return (pos - 1 + size) % size

        --pos;
        if (pos == -1)
            pos = size - 1;
        return pos;
    }

public:
    Deque(int size) : size(size) {
        array = new type[size];
    }

    bool isEmpty() { return added_elements == 0; }
    bool isFull() { return added_elements == size; }

    void enqueue_rear(int value) {
        assert(!isFull());
        array[rear] = value;
        rear = next(rear);
        ++added_elements;
    }
    void enqueue_front(int value) {
        assert(!isFull());
        front = prev(front);
        array[front] = value;
        ++added_elements;
    }

    type dequeue_front() {
        assert(!isEmpty());
        type ret = array[front];
        front = next(front);
        --added_elements;
        return ret;
    }
    type dequeue_rear() {
        assert(!isEmpty());
        rear = prev(rear);
        --added_elements;
        return array[rear];
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

////////// stack of queue /////////
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
    type dequeue() {
        assert(!isEmpty());
        type ret = array[front];
        front = next(front);
        --added_elements;
        return ret;
    }
    type front_value() {
        assert(!isEmpty());
        return array[front];
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

template <typename type>
class StackOfQueue {
    Queue<type> queue;
    int added_elements {};

    void insert_at_front(int item) {
        int oldSize = added_elements;
        queue.enqueue(item);
        while (oldSize--)
            queue.enqueue(queue.dequeue());
        // now all prev items are after the new one
        ++added_elements;
    }

public:
    StackOfQueue(int size) : queue(size) { }

    bool is_full() { return queue.isFull(); }
    bool is_empty() { return queue.isEmpty(); }

    void push(type item) { // O(n)
        insert_at_front(item);
    }
    type pop() {
        --added_elements;
        return queue.dequeue();
    }
    type top() {
        return queue.front_value();
    }
};

////////// queue of 2 stacks /////////
// dequeue in O(1)
template <typename T>
class Stack {
    int size {};
    int topidx {};
    T* array {};
public:
    Stack(int size) : size(size), topidx(-1) {
        array = new T[size];
    }
    ~Stack() {
        delete[] array;
    }

    bool is_full() {
        return (topidx == size -1);
    }
    bool is_empty() {
        return (topidx == -1);
    }

    void push(T item) {
        assert(!is_full());
        array[++topidx] = item;
    }
    T pop() {
        assert(!is_empty());
        return array[topidx--];
    }
    T top() {
        assert(!is_empty());
        return array[topidx];
    }

    void display() {
        for(int i = topidx; i >= 0; --i)
            cout << array[i] << ' ';
        cout << endl;
    }
};

template <typename type>
class QueueOf2Stacks1 {
    int added_elements {};
    Stack<type> stack1, stack2;

    void move(Stack<type>& from, Stack<type>& to) {
        while (!from.is_empty())
            to.push(from.pop());
    }

public:
    QueueOf2Stacks1(int size) : stack1(size), stack2(size) { }

    bool is_full() { return stack1.is_full(); }
    bool is_empty() { return stack1.is_empty(); }

    void enqueue(type item) { // O(n)
        // use stack1 as the main container, and stack2 as a temp one for emptizing

        move(stack1, stack2);
        stack1.push(item);
        move(stack2, stack1);

        ++added_elements;
    }
    int dequeue() {
        --added_elements;
        return stack1.pop();
    }
};

// enqueue in O(1)
template <typename type>
class QueueOf2Stacks2 {
    int size, added_elements {};
    Stack<type> stack1, stack2;

    void move(Stack<type>& from, Stack<type>& to) {
        while (!from.is_empty())
            to.push(from.pop());
    }

public:
    QueueOf2Stacks2(int size) : size(size), stack1(size), stack2(size) { }

    bool is_full() { return added_elements == size; }
    bool is_empty() { return added_elements == 0; }

    void enqueue(type item) {
        assert(!is_full());
        stack1.push(item);
        ++added_elements;
    }
    type dequeue() {
        assert(!is_empty());

        if (stack2.is_empty())
            move(stack1, stack2);
        type val = stack2.pop();

        --added_elements;
        return val;
    }
};

//////////// test & main ////////////

void test() {
    QueueOf2Stacks2<char> queue(10);
    for (int i = 65; i < 70; ++i) {
        queue.enqueue(char(i));
    }

    for (int i = 71; i < 75; ++i)
        queue.enqueue(char(i));

    while (!queue.is_empty())
        cout << queue.dequeue() << ' ';
}

int main() {
    test(); return 0;
}