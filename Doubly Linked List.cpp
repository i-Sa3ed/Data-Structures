/// doubly linked list code ///
// main advantage: obtaining the previous node in O(1) rather than O(n)

#include <iostream>
#include <cassert> // for "assert"
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int data;
    Node* next {};
    Node* prev {}; // previous node

    Node(int _data) : data(_data) {}
    ~Node() { cout << "Destroy: " << data << endl; }

    void set(Node* _next, Node* _prev) {
        this->next = _next;
        this->prev = _prev;
    }
};

class DoublyLinkedList {
    Node* head {};
    Node* tail {};
    int length = 0;

    vector<Node*> debug_data;

    /// utility functions:
    void link(Node* first, Node* second) {
        if(first) first->next = second;
        if(second) second->prev = first;
    }

    void debug_add_node(Node *node) {
        debug_data.push_back(node);
    }
    void debug_remove_node(Node *node) {
        auto it = find(debug_data.begin(), debug_data.end(), node);
        if(it == debug_data.end()) cout << "Node is not exist\n";
        else debug_data.erase(it);
    }

    void add_node(Node* node) {
        debug_add_node(node);
        length++;
    }
    void delete_node(Node* node) {
        debug_remove_node(node);
        length--;
        delete node;
    }

    void embed_after(Node* node_before, int value) {
        Node* middle = new Node(value);
        Node* node_after = node_before->next;

        link(node_before, middle);
        link(middle, node_after);

        add_node(middle);
    }

    Node* delete_and_link(Node* toDel) {
        Node* ret = toDel->prev;
        link(toDel->prev, toDel->next);
        delete_node(toDel);

        return ret;
    }

public:
    ~DoublyLinkedList() {
        while(head) {
            Node* coming = head->next;
            delete head;
            head = coming;
        }
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
        for(Node* cur = head; cur; cur = cur->next) {
                len++;
                assert(len < 1e4);
        };

        assert(len == length);
        assert(length == (int)debug_data.size());
    }
    string debug_to_string() const {
        string ret;
        for(Node* cur = head; cur; cur = cur->next) {
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

    /// insertion:
    void insert_end(int value) {
        Node* item = new Node(value);
        // add to the vector //

        if(!head)
            head = tail = item;
        else {
            link(tail, item);
            tail = item;
        }
        add_node(item);

        verify_data();
    }
    void insert_front(int value) {
        Node* item = new Node(value);
        if(!head)
            head = tail = item;
        else {
            link(item, head);
            head = item;
        }
        add_node(item);

        verify_data();
    }
    void insert_sorted(int value) {
        if(!length or head->data >= value)
            insert_front(value);
        else if(tail->data <= value)
            insert_end(value);
        else {
            for(Node* cur = head; cur; cur = cur->next) {
                if(cur->data >= value) { // found
                    embed_after(cur->prev, value);
                    break;
                }
            }
        }

        verify_data();
    }

    /// deletion:
    void pop_front() { // O(1) time - O(1) memory
        assert(length);

        Node* newHead = head->next;
        delete_node(head);
        head = newHead;

        if(head)
            head->prev = nullptr;
        else if(!length)
            tail = nullptr;

        verify_data();
    }
    void pop_back() { // O(1) time - O(1) memory
        assert(length);

        Node* newTail = tail->prev;
        delete_node(tail);
        tail = newTail;

        if(tail)
            tail->next = nullptr;
        else if(!length)
            head = nullptr;

        verify_data();
    }
    void delete_node_wit_key(int key) { // O(n) time - O(1) memory
        if(!length) return;
        else if(head->data == key) pop_front();
        else {
            for(Node* cur = head; cur; cur = cur->next) {
                if(cur->data == key) {
                    cur = delete_and_link(cur);
                    if(!cur->next) // handle case of tail
                        tail = cur;
                    break;
                }
            }
        }

        verify_data();
    }
    void delete_all_nodes_with_key(int key) { // O(n) time - O(1) memory
        while(head and head->data == key) pop_front();
        for(Node* cur = head; cur; cur = cur->next) {
            if(cur->data == key) {
                cur = delete_and_link(cur);
                if(!cur->next)
                    tail = cur;
            }
        }

        verify_data();
    }
    void delete_even_positions() { // O(n) time - O(1) memory
        if(length < 2) return;

        // cur : odd, cur.next : even
        for(Node* cur = head; cur and cur->next; cur = cur->next) {
            delete_and_link(cur->next);
            if(!cur->next)
                tail = cur;
        }

        verify_data();
    }
    void delete_odd_positions() { // O(n) time - O(1) memory
        if(!length) return;

        insert_front(-1); // dummy
        // now the odd positions converted to even ones ;)
        delete_even_positions();
        pop_front(); // delete the dummy value

        verify_data();
    }

    // is palindrome:
    bool is_palindrome() {
        Node* start = head, *last = tail;
        for (int i = 1; i <= length / 2; ++i)
            if(start->data != last->data)
                return false;

        return true;
    }

    /// printing:
    void print() {
        Node* cur = head; // to avoid changing 'head' value
        while(cur != nullptr) {
            cout << cur->data << ' ';
            cur = cur->next;
        }
        cout << endl;
    }
    void print_reserved() {
        for(Node* cur = tail; cur; cur = cur->prev) {
            cout << cur->data << ' ';
        }
        cout << endl;
    }
};

///////////// test ///////////////////

void test1() {
    DoublyLinkedList dll;
    dll.insert_front(10);

    if(dll.is_palindrome()) dll.print();
    else cout << "fail :(\n";
}

///////// main ////////////////////

int main()
{
    test1();


    cout << "\nno RTE ;)\n";
    return 0;
}