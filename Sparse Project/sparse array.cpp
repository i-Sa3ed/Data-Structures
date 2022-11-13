/// doubly linked list code ///
// main advantage: obtaining the previous ArrayNode in O(1) rather than O(n)

#include <iostream>
#include <cassert> // for "assert"
#include <vector>
#include <algorithm>

using namespace std;

struct ArrayNode {
    int data, index;
    ArrayNode* next {};
    ArrayNode* prev {}; // previous ArrayNode

    ArrayNode(int _data, int idx) : data(_data), index(idx) {}
    ~ArrayNode() { cout << "Destroy: " << data << endl; }

    void set(ArrayNode* _next, ArrayNode* _prev) {
        this->next = _next;
        this->prev = _prev;
    }
};

class ArrayLinkedList {
    ArrayNode* head {};
    ArrayNode* tail {};
    int length = 0;
    int array_length; // just useful for printing with zeros

    vector<ArrayNode*> debug_data;

    /// utility functions:
    void link(ArrayNode* first, ArrayNode* second) {
        if(first) first->next = second;
        if(second) second->prev = first;
    }

    void debug_add_ArrayNode(ArrayNode *ArrayNode) {
        debug_data.push_back(ArrayNode);
    }
    void debug_remove_ArrayNode(ArrayNode *ArrayNode) {
        auto it = find(debug_data.begin(), debug_data.end(), ArrayNode);
        if(it == debug_data.end()) cout << "ArrayNode is not exist\n";
        else debug_data.erase(it);
    }

    void add_ArrayNode(ArrayNode* ArrayNode) {
        debug_add_ArrayNode(ArrayNode);
        length++;
    }
    void delete_ArrayNode(ArrayNode* ArrayNode) {
        debug_remove_ArrayNode(ArrayNode);
        length--;
        delete ArrayNode;
    }

    ArrayNode* embed_after(ArrayNode* ArrayNode_before, int value, int idx) {
        ArrayNode* middle = new ArrayNode(value, idx);
        ArrayNode* ArrayNode_after = ArrayNode_before->next;

        link(ArrayNode_before, middle);

        if (!ArrayNode_after)
            tail = middle;
        else
            link(middle, ArrayNode_after);

        add_ArrayNode(middle);

        return middle;
    }

    // return the node of the given index:
    ArrayNode* get_index(int idx, bool create_if_missing) {
        ArrayNode* prev_idx = head;

        while (prev_idx->next and prev_idx->next->index < idx)
            prev_idx = prev_idx->next;

        bool found = prev_idx->next and prev_idx->next->index == idx;

        if (found)
            return prev_idx->next;
        if (!create_if_missing)
            return nullptr;

        return embed_after(prev_idx, 0, idx);
    }

    ArrayNode* delete_and_link(ArrayNode* toDel) {
        ArrayNode* ret = toDel->prev;
        link(toDel->prev, toDel->next);
        delete_ArrayNode(toDel);

        return ret; // prev
    }

public:
    ArrayLinkedList(int len) {
        this->array_length = len;

        auto dummy = new ArrayNode(0, -1);
        tail = head = dummy; // dummy node
        add_ArrayNode(dummy);
    }

    ~ArrayLinkedList() {
        while(head) {
            ArrayNode* coming = head->next;
            delete_ArrayNode(head);
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
        for(ArrayNode* cur = head; cur; cur = cur->next) {
                len++;
                assert(len < 1e4);
        };

        assert(len == length);
        assert(length == (int)debug_data.size());
    }
    string debug_to_string() const {
        string ret;
        for(ArrayNode* cur = head; cur; cur = cur->next) {
                ret += (to_string(cur->data) + ' ');
        };

        return ret;
    }
    // debug using the vector:
    void debug_print_ArrayNode(ArrayNode *ArrayNode, bool is_seperated = false) {
        if(is_seperated) cout << "Sep: ";
        if(ArrayNode == nullptr) {
            cout << "null\n";
            return;
        }

        cout << ArrayNode->data << ' ';
        if(ArrayNode->next == nullptr) cout << "X ";
        else cout << ArrayNode->next->data << ' ';

        // head and tail:
        if(ArrayNode == head) cout << "Head";
        else if(ArrayNode == tail) cout << "Tail";

        cout << '\n';
    }
    void debug_print_list() {
        int siz = (int)debug_data.size();
        for (int i = 0; i < siz; ++i) {
            debug_print_ArrayNode(debug_data[i]);
        }
        cout << "***************\n" << flush;
    }

    /// insertion:
    void insert_front(int value, int idx) {
        ArrayNode* item = new ArrayNode(value, idx);
        if(!head)
            head = tail = item;
        else {
            link(item, head);
            head = item;
        }
        add_ArrayNode(item);

        verify_data();
    }
    void insert_front(ArrayNode* item) { // overloading
        if(!head)
            head = tail = item;
        else {
            link(item, head);
            head = item;
        }
        add_ArrayNode(item);

        verify_data();
    }
    void insert_end(int value, int idx) {
        ArrayNode* item = new ArrayNode(value, idx);
        if(!head)
            head = tail = item;
        else {
            link(tail, item);
            tail = item;
        }
        add_ArrayNode(item);

        verify_data();
    }

    /// set and get:
    void set(int value, int idx) {
        assert(idx >= 0 and idx < array_length);

        get_index(idx, true)->data = value;

        verify_data();
    }
    int get_value(int idx) {
        assert(idx >= 0 and idx < array_length);

        ArrayNode* node = get_index(idx, false);
        if (!node) return 0;
        else return node->data;
    }

    /// adding:
    void add(ArrayLinkedList& other) { // O(n) time - O(n) memory
        assert(array_length == other.array_length);

        ArrayNode *prev1 = head, *prev2 = other.head;

        while (prev2->next) {
            while (prev1->next and prev2->next->index > prev1->next->index)
                prev1 = prev1->next;

            if (prev1->next and prev1->next->index == prev2->next->index)
                prev1->next->data += prev2->next->data;
            else // not found
                embed_after(prev1, prev2->next->data, prev2->next->index);

            prev2 = prev2->next;
        }

        verify_data();
    }

    /// printing:
    void print_valid_items() {
        for (ArrayNode* cur = head; cur; cur = cur->next) {
            cout << cur->data << ' ';
        }
        cout << endl;
    }
    void print_total_array() { // O(n) time - O(1) memory
        ArrayNode* cur = head->next;
        for (int i = 0; i < array_length; ++i) {
            if (cur and cur->index == i) {
                cout << cur->data << ' ';
                cur = cur->next;
            }
            else cout << "0 ";
        }

        cout << endl;
    }

};

///////////// test ///////////////////

void test1() {
    ArrayLinkedList array(10); // empty

    cout << "array 1: \n";
    array.print_total_array();

    ArrayLinkedList other(10);
    for (int i = 1; i <= 10; i += i) {
        other.set(i*i, i);
    }
    other.set(81, 9); // tail
    other.set(2, 0); // head

    cout << "array 2:\n";
    other.print_total_array();

    /// adding:
    array.add(other);
    cout << "after adding: \n";
    array.print_total_array();

}

///////// main ////////////////////

int main()
{
    test1();


    cout << "\nno RTE ;)\n";
    return 0;
}