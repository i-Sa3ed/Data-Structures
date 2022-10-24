//
// Created by A on 10/24/2022.
//

#include <bits/stdc++.h>
using namespace std;

/*
 * definitions:
 *  - singly: b. it has a "single" arrow to the next node only
 * advantages:
 *  - appending elements dynamically in O(1) time
 */

struct node {
    int data;
    node* next { }; // ptr from the same type

    node(int _data) : data(_data) {} // constructor
};

class LinkedList {
    node* head {};
    node* tail {};
    int length = 0; // don't forget to update it

public: // the operations on data
    void print() {
        node* tmpHead = head; // to avoid changing 'head' value
        while(tmpHead != nullptr) {
            cout << tmpHead->data << ' ';
            tmpHead = tmpHead->next;
        }
        cout << endl;
    }

    void insert_end(int value) {
        node* nod = new node(value); // we need to delete it

        if(head == nullptr) {
            head = tail = nod;
        } else {
            tail->next = nod;
            tail = nod;
        }

        length++;
    }
};

// display nodes:
void print_iterate(node* head) {
    for(; head != nullptr; head = head->next)
        cout << head->data << ' ';
    cout << endl;
}
void print_recurse(node* head) {
    if(head == nullptr) {
        cout << endl;
        return;
    }

    cout << head->data << ' ';
    print_recurse(head->next);
}
void print_recurse_reverse(node* head) {
    if(head == nullptr) {
        return;
    }

    print_recurse_reverse(head->next);
    cout << head->data << ' ';
}

// search:
node* find(node* head, int val) {
    while(head != nullptr) {
        if(val == head->data) return head;
        else head = head->next;
    }
    return nullptr;
}

int main()
{
    // start with the nodes:
    node* node1 = new node(10); // head
    node* node2 = new node(20);
    node* node3 = new node(30);
    // link them:
    node1->next = node2;
    node2->next = node3;
    node3->next = nullptr;

    // displaying nodes:
    node* head = node1;

    print_iterate(head);
    /// why head didn't change, even if it's a pass by ptr?!
    print_recurse(head);
    print_recurse_reverse(head);
    cout << endl;

    // search:
    auto res = find(head, 1);
    cout << (res == nullptr? "not found\n" : "found!\n");

    // try insert:
    LinkedList list;
    for (int i = 1; i <= 10; ++i) {
        list.insert_end(i*i);
    }
    list.print();

    return 0;
}