//
// Created by A on 10/24/2022.
//

#include <bits/stdc++.h>
using namespace std;

/*
 * definitions:
 *  - singly: b. it has a "single" arrow to the next Node only
 * advantages:
 *  - appending elements dynamically in O(1) time
 */

#define forNode for(Node *cur = head; cur; cur = cur->next)

struct Node {
    int data;
    Node* next { }; // ptr from the same type

    Node(int _data) : data(_data) {} // constructor
    ~Node() { // destructor
        cout << "Destroy the value: " << data << " at the address: " << this << endl;
    }
};

/// function on Nodes ///

// display Nodes:
void print_iterate(Node *head) { // notice that: passing by ptr is not like passing by ptr, e.g. u can pass the pointers by ref !
    for(; head != nullptr; head = head->next)
        cout << head->data << ' ';
    cout << endl;
}
void print_recurse(Node* head) {
    if(head == nullptr) {
        cout << endl;
        return;
    }

    cout << head->data << ' ';
    print_recurse(head->next);
}
void print_recurse_reverse(Node* head) {
    if(head == nullptr) {
        return;
    }

    print_recurse_reverse(head->next);
    cout << head->data << ' ';
}

// search:
Node* find(Node* head, int val) {
    while(head != nullptr) {
        if(val == head->data) return head;
        else head = head->next;
    }
    return nullptr;
}

/////////////////////////////////////

class LinkedList {
    Node* head {};
    Node* tail {};
    int length = 0; // don't forget to update it

    // simple functions:
    void add_node(Node* node) {
        debug_add_node(node);
        length++;
    }
    void delete_node(Node* node) {
        debug_remove_node(node);
        length--;
        delete node;
    }

    vector<Node*> debug_data; // to keep the elements of the linked list safe in case of any corruptness
    void debug_add_node(Node *node) {
        debug_data.push_back(node);
    }
    void debug_remove_node(Node *node) {
        auto it = find(debug_data.begin(), debug_data.end(), node);
        if(it == debug_data.end()) cout << "Node is not exist\n";
        else debug_data.erase(it);
    }

    // utility to test "remove_cycle" function
    void create_cycle() {
        for (int i = 0; i < 4; ++i)
            insert_end(i);
        Node* now = tail;
        for (int i = 0; i < 3; ++i)
            insert_end(14 + i);
        tail->next = now;// cycle
    }

public: // the operations on data
    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;
    LinkedList &operator=(const LinkedList &another) = delete;

    ~LinkedList() { // O(n) time - O(1) memory
        while(head) {
            Node* cur = head->next;
            delete head;
            head = cur;
        }
    }

    void print() {
        Node* cur = head; // to avoid changing 'head' value
        while(cur != nullptr) {
            cout << cur->data << ' ';
            cur = cur->next;
        }
        cout << endl;
    }

    /// insertion:
    void insert_end(int value) {
        Node* nod = new Node(value); // we need to delete it

        if(head == nullptr) {
            head = tail = nod;
        } else {
            tail->next = nod;
            tail = nod;
        }

        add_node(nod);

        verify_data();
    }

    // Hashing challenge:
    void remove_cycle() {
        unordered_set<Node*> visited;
        for (Node *prev = nullptr, *cur = head; cur; prev = cur, cur = cur->next) {
            if (visited.count(cur)) { // visited before => cycle
                prev->next = nullptr;
                prev = tail;
                break;
            }
        }
    }

    void insert_front(int value) {
        Node* front = new Node(value);
        if (head == nullptr) {
            head = tail = front;
        }
        else {
            front->next = head;
            head = front;
        }

        add_node(front);

        verify_data();
    } // O(1)
    void insert_sorted(int value) { // O(n) time - O(1) memory
        if(!length or head->data >= value)
            insert_front(value);
        else if(tail->data <= value)
            insert_end(value);
        else {
            for(Node* cur = head; cur; cur = cur->next) {
                if(cur->next->data >= value) {
                    Node* node = new Node(value);
                    // linking:
                    node->next = cur->next;
                    cur->next = node;

                    add_node(node);

                    break;
                }
            }

        }

        verify_data();
    }

    /// deletion:
    void pop_front() {
        assert(length);

        Node* tmp = head;
        head = head->next;
        delete_node(tmp);

        if(!head) tail = nullptr;

        verify_data();
    }
    void pop_back() { // O(length) time - O(1) memory
        assert(length);

        if(length == 1) {
            pop_front();
        }
        else {
            Node* prev = get_item(length - 1); // O(idx)
            delete_node(tail);
            tail = prev;
            tail->next = nullptr; // don't forget
        }

        verify_data();
    }
    void delete_nth(int n) { // O(length) time - O(1) memory
        assert(n > 0 and n <= length);

        if(n == 1) pop_front();
        else if(n == length) pop_back();
        else {
            Node* prev = get_item(n - 1); // O(idx)
            Node* nth = prev->next;
            prev->next = nth->next;

            delete_node(nth);
        }

        verify_data();
    }
    bool delete_node_without_head(Node* node) { /// from interview Q
        if(node == tail) return false; // but with a condition: it's not tail

        node->data = node->next->data;
        Node* temp = node->next;
        node->next = node->next->next;

        delete_node(temp);
        return true;
    }
    void delete_node_with_key(int key) { // O(n) time - O(1) memory
        if(!length) {
            cout << "Empty list!\n";
            return;
        }

        if(head->data == key) {
            pop_front();
            return;
        }

        bool found = false;
        for(Node* prev = head; prev->next; prev = prev->next) {
            if(prev->next->data == key) {
                found = true;

                Node* toDel = prev->next;
                prev->next = prev->next->next;
                delete_node(toDel);

                if(!prev->next) tail = prev; // handle case of tail deletion

                break;
            }
        }

        if(!found) cout << "value not found!\n";

        verify_data();
    }
    void delete_even_positions() {
        if(length < 2) return;


        Node* nextOdd = head;
        Node* prevOdd;
        while (nextOdd and nextOdd->next) {
            Node* todel = nextOdd->next; // keeping
            // linking:
            prevOdd = nextOdd;
            nextOdd = nextOdd->next->next;
            prevOdd->next = nextOdd;
            // deleting:
            delete_node(todel);
        }

        if (!nextOdd) tail = prevOdd;

        verify_data();
    }
    void clear() {
        while(head != nullptr) {
            Node* tmp = head;
            head = head->next;
            delete_node(tmp);
        }
        tail = nullptr;

        verify_data();
    }

    /// get items:
    Node* get_item(int idx_1) {
        // notice that the index is one based

        if(idx_1 > length or idx_1 < 1) return nullptr;
        else {
            Node* cur = head;
            for (int i = 1; i < idx_1; ++i) {
                cur = cur->next;
            }
            return cur;
        }
    } // O(idx)
    Node* get_item_back(int idx_1) {
        // notice that the index is one based

        if(idx_1 > length or idx_1 < 1) return nullptr;
        else return get_item(length - idx_1 + 1);
    } // O(idx)

    /// search:
    int search(int val) {
        // returning the index 0-based, or -1

        Node* cur = head;
        int idx = 0;
        for( ; cur; cur = cur->next, idx++)
            if(cur->data == val) return idx;

        return -1; //not found
    }
    int improved_search(int val) {
        // search with shifting to the left

        int idx = 0;
        for(Node *cur = head, *prev = nullptr; cur; prev = cur, cur = cur->next, idx++) { // notice the order of increment
            if(cur->data == val) {
                if(!prev) return idx;

                swap(prev->data, cur->data); // notice
                return (idx - 1);
            }
        }

        return -1;
    }

    /// comparison:
    bool is_same(const LinkedList& other) {
        Node* other_h = other.head, *cur_h = head;
        for( ; cur_h and other_h; cur_h = cur_h->next, other_h = other_h->next)
            if(cur_h->data != other_h->data) return false;

        return !cur_h and !other_h;

        verify_data();
    }

    /// swapping:
private :
    void swap_pairs_recurse(Node *&node) {
        if(!node or !node->next) return;

        swap(node->data, node->next->data);
        swap_pairs_recurse(node->next->next);
    }
public:
    void swap_pairs() { // O(n) time - O(1) memory (b. i pass pointers by ref ;)
        swap_pairs_recurse(head);
    }

    /// reversing:
public:
    void reverse() {
        if(length <= 1) return;

        tail = head;
        Node* prev = head;
        head = head->next;

        while (head) {
            // store next & reverse the link:
            Node* next = head->next;
            head->next = prev;

            // move step forward:
            prev = head;
            head = next;
        }
        // finalize head and tail:
        head = prev; // b. head was null
        tail->next = nullptr;

        verify_data();
    }

    /// debugging and testing ///
    void verify_data() {
        // my data: head, tail, length

        if(length == 0) assert(head == nullptr and tail == nullptr);
        else {
            assert(head and tail);
            if(length == 1) assert(head == tail);
            else assert(head != tail);

            assert(!tail->next);
        }

        int len = 0;
        forNode {
            len++;
            assert(len < 1e4);
        };

        assert(len == length);
        assert(length == (int)debug_data.size());
    }
    string debug_to_string() const {
        string ret;
        forNode {
            ret += (to_string(cur->data) + ' ');
        };

        return ret;
    }

    // debug using the vector:
    void debug_print_node(Node *node, bool is_seperated = false) {
        if(is_seperated) cout << "Sep: ";
        if(node == nullptr) {
            cout << "null\n";
            return;
        }

        cout << node->data << ' ';
        if(node->next == nullptr) cout << "X ";
        else cout << node->next->data << ' ';

        // head and tail:
        if(node == head) cout << "Head";
        else if(node == tail) cout << "Tail";

        cout << '\n';
    }
    void debug_print_list() {
        int siz = (int)debug_data.size();
        for (int i = 0; i < siz; ++i) {
            debug_print_node(debug_data[i]);
        }
        cout << "***************\n" << flush;
    }
};

///////////////////////////////////////////////////////

class ReducedLinkedList {
    Node *head {};
public:
    void print() {
        for(Node *cur = head; cur != nullptr; cur = cur->next)
            cout << cur->data << ' ';
        cout << endl;
    } // O(n)

    void add_element(int value) {
        Node *node = new Node(value);
        node->next = head;
        head = node;
    } // O(1)

    Node* get_tail() {
        if(head == nullptr) return nullptr;

        Node *cur;
        for(cur = head; cur->next; cur = cur->next)
            ;
        return cur;
    } // O(n)
};

////////////////////// test ///////////////////////////////////

void test1() {
    LinkedList list;

    /// inset sorted:

    // empty list:
    list.insert_sorted(14);
    list.print();

    // smallest:
    list.insert_sorted(10);
    list.print();

    // more than one:
    for(int i = 5; i <= 20; i += 5)
        list.insert_sorted(i);
    list.print();

}

//////////////////// main ////////////////////////////////////

int main()
{
    test1();

    cout << "\nNo RTE ;)\n"; // just for asserting
    return 0;
}

/*
 * TO DO:
 * clear()
 */