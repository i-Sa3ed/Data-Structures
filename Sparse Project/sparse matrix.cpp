
#include <iostream>
#include <cassert> // for "assert"
#include <vector>
#include <algorithm>

using namespace std;

struct ColumnNode {
    int data, column;
    ColumnNode* next {};
    ColumnNode* prev {}; // previous ArrayNode

    ColumnNode(int _data, int col) : data(_data), column(col) {}
    ~ColumnNode() { cout << "Destroy: " << data << endl; }
};

class ColumnLinkedList {
    ColumnNode* head {};
    ColumnNode* tail {};
    int length {};
    int cols {}; // just useful for printing with zeros

    vector<ColumnNode*> debug_data;

    /// utility functions:
    void link(ColumnNode* first, ColumnNode* second) {
        if(first) first->next = second;
        if(second) second->prev = first;
    }

    void debug_add_ArrayNode(ColumnNode *ArrayNode) {
        debug_data.push_back(ArrayNode);
    }
    void debug_remove_ArrayNode(ColumnNode *ArrayNode) {
        auto it = find(debug_data.begin(), debug_data.end(), ArrayNode);
        if(it == debug_data.end()) cout << "ArrayNode is not exist\n";
        else debug_data.erase(it);
    }

    void add_ArrayNode(ColumnNode* ArrayNode) {
        debug_add_ArrayNode(ArrayNode);
        length++;
    }
    void delete_ArrayNode(ColumnNode* ArrayNode) {
        debug_remove_ArrayNode(ArrayNode);
        length--;
        delete ArrayNode;
    }

    ColumnNode* embed_after(ColumnNode* ArrayNode_before, int value, int idx) {
        ColumnNode* middle = new ColumnNode(value, idx);
        ColumnNode* ArrayNode_after = ArrayNode_before->next;

        link(ArrayNode_before, middle);

        if (!ArrayNode_after)
            tail = middle;
        else
            link(middle, ArrayNode_after);

        add_ArrayNode(middle);

        return middle;
    }

    // return the node of the given index:
    ColumnNode* get_index(int idx, bool create_if_missing) {
        ColumnNode* prev_idx = head;

        while (prev_idx->next and prev_idx->next->column < idx)
            prev_idx = prev_idx->next;

        bool found = prev_idx->next and prev_idx->next->column == idx;

        if (found)
            return prev_idx->next;
        if (!create_if_missing)
            return nullptr;

        return embed_after(prev_idx, 0, idx);
    }

    ColumnNode* delete_and_link(ColumnNode* toDel) {
        ColumnNode* ret = toDel->prev;
        link(toDel->prev, toDel->next);
        delete_ArrayNode(toDel);

        return ret; // prev
    }

public:
    ColumnLinkedList(int len) {
        this->cols = len;

        tail = head = new ColumnNode(0, -1);
        add_ArrayNode(head);
    }

    ~ColumnLinkedList() {
        while(head) {
            ColumnNode* coming = head->next;
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
        for(ColumnNode* cur = head; cur; cur = cur->next) {
                len++;
                assert(len < 1e4);
        };

        assert(len == length);
        assert(length == (int)debug_data.size());
    }
    string debug_to_string() const {
        string ret;
        for(ColumnNode* cur = head; cur; cur = cur->next) {
                ret += (to_string(cur->data) + ' ');
        };

        return ret;
    }
    // debug using the vector:
    void debug_print_ArrayNode(ColumnNode *ArrayNode, bool is_seperated = false) {
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
        ColumnNode* item = new ColumnNode(value, idx);
        if(!head)
            head = tail = item;
        else {
            link(item, head);
            head = item;
        }
        add_ArrayNode(item);

        verify_data();
    }
    void insert_front(ColumnNode* item) { // overloading
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
        ColumnNode* item = new ColumnNode(value, idx);
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
        assert(idx >= 0 and idx < cols);

        get_index(idx, true)->data = value;

        verify_data();
    }
    int get_value(int idx) {
        assert(idx >= 0 and idx < cols);

        ColumnNode* node = get_index(idx, false);
        if (!node) return 0;
        else return node->data;
    }

    /// adding:
    void add(ColumnLinkedList& other) { // O(n) time - O(n) memory
        assert(cols == other.cols);

        ColumnNode *prev1 = head, *prev2 = other.head;

        while (prev2->next) {
            while (prev1->next and prev2->next->column > prev1->next->column)
                prev1 = prev1->next;

            if (prev1->next and prev1->next->column == prev2->next->column)
                prev1->next->data += prev2->next->data;
            else // not found
                embed_after(prev1, prev2->next->data, prev2->next->column);

            prev2 = prev2->next;
        }

        verify_data();
    }

    /// printing:
    void print_valid_items() {
        for (ColumnNode* cur = head->next; cur; cur = cur->next) {
            cout << cur->data << ' ';
        }
        cout << endl;
    }
    void print_total_array() { // O(n) time - O(1) memory
        ColumnNode* cur = head->next;
        for (int i = 0; i < cols; ++i) {
            if (cur and cur->column == i) {
                cout << cur->data << ' ';
                cur = cur->next;
            }
            else cout << "0 ";
        }

        cout << endl;
    }

};

///////////// Matrix /////////////////////

struct RowNode {
    int row;
    ColumnLinkedList thisRow;

    RowNode* next {};
    RowNode* prev {};

    RowNode(int _row, int _cols) : row(_row), thisRow(_cols) { }
};

class SparseMatrix {
    RowNode* head = {};
    RowNode* tail = {};
    int length = 0;

    int rows = 0, cols = 0;

    /// Utility functions:
    void link(RowNode* first, RowNode* second) {
        if (first)
            first->next = second;
        if (second)
            second->prev = first;
    }

    RowNode* embed_row_after(RowNode* before, int row) {
        RowNode* after = before->next;
        RowNode* middle = new RowNode(row, cols);

        link(before, middle);
        if (after)
            link(middle, after);
        else
            tail = middle;
        ++length;

        return middle;
    }

    // return the arrayNode of the given index:
    RowNode* get_row(int row, bool create_if_missing) {
        RowNode* prev_idx = head;

        while (prev_idx->next and prev_idx->next->row < row)
            prev_idx = prev_idx->next;

        bool found = prev_idx->next and prev_idx->next->row == row;

        if (found)
            return prev_idx->next;
        if (!create_if_missing)
            return nullptr;

        return embed_row_after(prev_idx, row);
    }

public:
    SparseMatrix(int r, int c) : rows(r), cols(c) {
        tail = head = new RowNode(cols, -1);
        ++length;
    }

    /// set & get:
    void set(int value, int row, int col) {
        assert(row >= 0 and row < rows);
        assert(col >= 0 and col < cols);

        RowNode* rowNode = get_row(row, true);
        rowNode->thisRow.set(value, col);
    }
    int get_value(int row, int col) {
        assert(row >= 0 and row < rows);
        assert(col >= 0 and col < cols);

        RowNode* rowNode = get_row(row, false);
        if (!rowNode)
            return 0;
        else
            return rowNode->thisRow.get_value(col);
    }

    /// add:
    void add(SparseMatrix& other) {
        assert(rows == other.rows and cols == other.cols);

        RowNode* prv1 = head;
        RowNode* prv2 = other.head;

        while(prv2->next) {
            while (prv1->next and prv1->next->row < prv2->next->row)
                prv1 = prv1->next;

            if (prv1->next->row == prv2->next->row)
                prv1->next->thisRow.add(prv2->next->thisRow);
            else {
                embed_row_after(prv1, prv2->next->row)->thisRow.add(prv2->next->thisRow);
            }

            prv2 = prv2->next;
        }
    }

    /// printing:
    void print_valid_elements() {
        RowNode* row = head->next;
        while (row) {
            row->thisRow.print_valid_items();
            row = row->next;
        }
    }
    void print_all_matrix() {
        string zeroRow;
        for (int i = 1; i <= cols; ++i) {
            zeroRow += "0 ";
        }

        RowNode* curRow = head->next;
        for (int i = 0; i < rows; ++i) {
            if (curRow and curRow->row == i) {
                curRow->thisRow.print_total_array();
                curRow = curRow->next;
            }
            else
                cout << zeroRow << endl;
        }
    }
};

///////////// test ///////////////////

void test1() {

    SparseMatrix sparse(3, 6);
    SparseMatrix sparse2(3, 6);

    sparse.set(14, 1, 4);
    sparse.set(15, 1, 3);
    sparse.set(1, 0, 4);
    sparse.set(7, 2, 5);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 6; j += 3) {
            sparse2.set(i + j, i, j);
        }
    }
    sparse2.set(7, 1, 5);

    sparse.print_all_matrix();
    cout << "...........\n";
    sparse2.print_all_matrix();
    cout << "...........\n";
    //sparse2.print_valid_elements();

    sparse.add(sparse2);


    sparse.print_all_matrix();

}

///////// main ////////////////////

int main()
{
    test1();


    cout << "\nno RTE ;)\n";
    return 0;
}