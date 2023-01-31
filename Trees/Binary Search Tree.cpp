//
// Created by Saeed on 1/25/2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>
#include <queue>

using namespace std;

template <typename type>
class BST {
    type data {};
    BST* left {};
    BST* right {};

    /// Utility functions:
    void get_inorder(vector<type>& vec) { // O(n)
        if (left) left->get_inorder(vec);
        vec.push_back(data);
        if (right) right->get_inorder(vec);
    }
    BST* get_next(vector<BST*>& ancestors) {
        if (ancestors.empty()) return nullptr;

        BST* node = ancestors.back();
        ancestors.pop_back();
        return node;
    }

    BST* min_node() {
        while (left)
            return left->min_node();
        return this;
    }
    BST* max_node() {
        while (right)
            return right->max_node();
        return this;
    }

    void special_delete(BST* child) {
        data = child->data;
        left = child->left;
        right = child->right;
        delete child;
    }

    // delete the target node in the tree and return the updated tree
    // the updated one is used to re-link with children..
    BST* delete_node(type target, BST* node) {
        if (!node)
            return nullptr;

        if (target < node->data)
            node->left = delete_node(target, node->left); // update left
        else if (target > node->data)
            node->right = delete_node(target, node->right);
        else { // found the node to be deleted !
            if (!node->left and !node->right) { // case 1: no children
                delete node;
                node = nullptr;
            }
            else if (!node->right) // case 2: one left child
                node->special_delete(node->left);
            else if (!node->left) // case 2: one right child
                node->special_delete(node->right);
            else { // case 3: 2 children => use Successor
                BST* suc = node->right->min_node();
                node->data = suc->data; // copy suc data
                node->right = delete_node(node->data, node->right); // remove the original suc
            }
        }

        return node;
    }

    // delete a node using predecessor approach
    BST* delete_node_p(type target, BST* node) {
        if (!node)
            return nullptr;

        if (target < node->data)
            node->left = delete_node(target, node->left); // update left
        else if (target > node->data)
            node->right = delete_node(target, node->right);
        else { // found the node to be deleted !
            if (!node->left and !node->right) { // case 1: no children
                delete node;
                node = nullptr;
            }
            else if (!node->right) // case 2: one left child
                node->special_delete(node->left);
            else if (!node->left) // case 2: one right child
                node->special_delete(node->right);
            else { // case 3: 2 children => use Predecessor
                BST* pre = node->left->max_node();
                node->data = pre->data; // copy suc data
                node->left = delete_node_p(node->data, node->left);
            }
        }

        return node;
    }

    // delete the node without a recursive call:
    BST* delete_node_local(type target, BST* node) {
        if (!node)
            return nullptr;

        if (target < node->data)
            node->left = delete_node(target, node->left); // update left
        else if (target > node->data)
            node->right = delete_node(target, node->right);
        else { // found the node to be deleted !
            if (!node->left and !node->right) { // case 1: no children
                delete node;
                node = nullptr;
            }
            else if (!node->right) // case 2: one left child
                node->special_delete(node->left);
            else if (!node->left) // case 2: one right child
                node->special_delete(node->right);
            else { // case 3: 2 children => use Successor
                BST *parent = node, *child = node->right;
                while (child->left)
                    parent = child, child = child->left;

                // now: child = successor
                node->data = child->data;

                // now we need to relink the parent with child.right:
                if (parent->right == child) // the successor is = node.right
                    parent->right = child->right;
                else // successor is definitely on the left
                    parent->left = child->right;

                delete child;
                // no need to set to NULL b. it will be never used
            }
        }

        return node;
    }

public:
    BST(type data, BST* left = nullptr, BST* right = nullptr) :
    data(data), left(left), right(right) { }


    void insert(type target) { // O(h) time and memory
        // the memory is for the auxiliary stack space (recursive calls)

        if (target < data) {
            if (left) left->insert(target);
            else left = new BST<type>(target);
        }
        else {
            if (right) right->insert(target);
            else right = new BST<type>(target);
        }
    }

    bool search(type target) { // O(h)
        if (data == target) return true;

        if (target < data)
            return left and left->search(target);
        return right and right->search(target);
    }

    void print_inorder() {
        if (left)
            left->print_inorder();
        cout << data << ' ';
        if (right)
            right->print_inorder();
    }

    type min_value() {
        // min value = the most left node

        while (left)
            return left->min_value();
        return data;
    }
    type max_value() {
        // max value = the most right node

        while (right)
            return right->max_value();
        return data;
    }

    /// Easy challenges:
    bool search_iterative(type target) {
        BST* cur = this;
        while (cur) {
            if (cur->data == target) return true;
            else if (cur->data < target) cur = cur->right;
            else cur = cur->left;
        }

        return false;
    }

    bool is_bst1(int mn = INT_MIN, int mx = INT_MAX) { // O(n)
        bool status = mn < data and data < mx;
        if (!status)
            return false;

        bool left_bst = !left or left->is_bst1(mn, data);
        if (!left_bst)
            return false;

        bool right_bst = !right or right->is_bst1(data, mx);
        return right_bst;

        // notice that we do a check after each step to avoid wasting time
        // i.e. no need to check 'right' if the left is already not bst..
    }
    bool is_bst2() { // O(n) time and memory
        // get the inorder traversal and check if sorted
        vector<type> vec;
        get_inorder(vec);
        return is_sorted(vec.begin(), vec.end());
    }

    type kth_smallest(int& k) { // O(n)
        if (k == 0) return -1234;

        if (left) {
            type res = left->kth_smallest(k);
            if (k == 0)
                return res;
        }

        --k; // current node
        if (k == 0) // found it !
            return data;

        if (right)
            return right->kth_smallest(k);
        // we say:
            // find in the left side, found it: ok,
            // otherwise: find in the right one

        return -1234;
    }

    type lca(type x, type y) { // O(h)
        // if both on left or right subtree, then lca must be in the subtree
        // otherwise this node is lca: x is in a branch and y in another

        if (x < data and y < data)
            return left and left->lca(x, y);
        if (x > data and y > data)
            return right and right->lca(x, y);

        return data;
    }
    //Tip: to find the path between any 2 nodes: you get their lca
    //then path (lca, a) + path(lca, b)

    /// Successor ///

    bool find_chain(vector<BST*>& ancestors, type target) {
        // modified find

        ancestors.push_back(this);

        if (target == data)
            return true;

        if (target < data)
            return left and left->find_chain(ancestors, target);
        return right and right->find_chain(ancestors, target);
    }
    pair<bool, type> successor(type target) {
        vector<BST*> ancestors;

        if (!find_chain(ancestors, target)) // target not found
            return {false, -1};

        BST* child = get_next(ancestors);
        if (child->right) // case 1
            return {true, child->right->min_value()};

        // case 2:
        BST* parent = get_next(ancestors);
        while (parent and parent->right == child)
            child = parent, parent = get_next(ancestors); // keep moving up

        if (parent)
            return {true, parent->data};
        return {false, -1}; // target is the max value
    }


    /// medium challenges:
    void successor_queries(deque<type>& queries, vector<type>& successors, vector<type>& traversal) {
        // modified inorder traversal, just follow up the next query
        // traversal => part of inorder traversal, contained only the traversed elements

        if (queries.empty()) // to avoid redundant traversing
            return;

        if (left and queries.front() < data) {
            left->successor_queries(queries, successors, traversal);

            if (queries.empty())
                return;
        }

        // the last element is the requested element,
        // then the cur element is the successor
        if (!traversal.empty() and traversal.back() == queries.front()) {
            successors.push_back(data);
            queries.pop_front();

            if (queries.empty())
                return;
        }
        traversal.push_back(data);

        if (right and queries.front() >= data) // observe the (=) sign
            right->successor_queries(queries, successors, traversal);
    }

    /// Deletion ///
    void delete_value(type target) {
        if (target == data and !left and !right)
            return; // can't remove the root (make the tree empty)
        delete_node_local(target, this);
    }
};

/// challenge: balanced BST
template <typename type>
BST<type>* build_balanced_bst(vector<type>& values, int l = 0, int r = -10) { // O(n)
    if (r == -10) {
        r = values.size() - 1;
    }

    if (l > r)
        return nullptr;

    // make use of the Divide and Conquer approach
    int mid = (l + r) / 2;
    BST<type>* left = build_balanced_bst(values, l, mid - 1);
    BST<type>* right = build_balanced_bst(values, mid + 1, r);
    BST<type>* root = new BST<type>(values[mid], left, right);

    return root;
}

/// challenge: is degenerate
template <typename type>
bool is_degenerate(vector<type>& preorder) {
    if (preorder.empty()) return false; // empty

    type mn, mx;
    mn = 'a' - 1, mx = 'z' + 1;
    for(int i = 1; i < preorder.size(); ++i) {
        if (preorder[i] < mn or preorder[i] > mx)
            return false;

        if (preorder[i] < preorder[i - 1]) mx = preorder[i - 1] - 1; // the next subtree must be strictly smaller than me
        else mn = preorder[i - 1] + 1;
    }

    return true;
}

/// challenge: parent link
class BST_Parent {
    int data{};
    BST_Parent *left{};
    BST_Parent *right{};
    BST_Parent *parent{};

public:
    BST_Parent(int data, BST_Parent *parent = nullptr) :
            data(data), parent(parent) {}

    void insert(int target) { // O(h) time and memory
        // the memory is for the auxiliary stack space (recursive calls)

        if (target < data) {
            if (left) left->insert(target);
            else left = new BST_Parent(target, this);
        } else {
            if (right) right->insert(target);
            else right = new BST_Parent(target, this);
        }
    }

    void print_parent(int target) {
        if (target == data) {
            if (parent)
                cout << parent->data << endl;
            else cout << -1 << endl;
            return;
        }

        if (target < data and left)
            left->print_parent(target);
        if (target > data and right)
            right->print_parent(target);
    }

    void print_inorder() {
        if (left)
            left->print_inorder();
        cout << data << ' ';
        if (right)
            right->print_inorder();
    }

    BST_Parent* find(int target) { // O(h)
        if (data == target) return this;

        if (target < data)
            return left ? left->find(target) : nullptr;
        return right ? right->find(target) : nullptr;
    }

    int min_value() {
        // min value = the most left node

        while (left)
            return left->min_value();
        return data;
    }
    pair<bool, int> successor(int target) {
        BST_Parent* node = find(target);
        if (!node)
            return {false, -1};

        if (node->right) // case 1
            return {true, node->right->min_value()};

        // case 2: utilize parent feature
        while (node->parent and node->parent->right == node)
            node = node->parent;

        if (node->parent)
            return {true, node->parent->data};
        return {false, -1}; // node = max value
    }
};

/// challenge: rewrite the class using internal Node struct
// pros: easily delete the root (more flexible code)
// cons: duplicate functions for node*

template <typename type>
class BinarySearchTree {
    struct BinaryNode {
        type data;
        BinaryNode *left {};
        BinaryNode *right{};

        BinaryNode(type data) : data(data) { }
    };
    BinaryNode* root {};

    void insert_node(BinaryNode* node, type target) {
        if (target < node->data) {
            if (node->left)
                insert_node(node->left, target);
            else node->left = new BinaryNode(target);
        }
        else {
            if (node->right)
                insert_node(node->right, target);
            else node->right = new BinaryNode(target);
        }
    }

    bool search_node(BinaryNode* node, type target) {
        if (target < node->data)
            return node->left and search_node(node->left, target);
        else if (target > node->data)
            return node->right and search_node(node->right, target);

        return true;
    }

    void print_inorder_node(BinaryNode* node) {
        if (node->left)
            print_inorder_node(node->left);
        cout << node->data << ' ';
        if (node->right)
            print_inorder_node(node->right);
    }

    void level_order_trav_node() {
        queue<BinaryNode*> levels;
        levels.push(root);

        int l = 0;
        while (!levels.empty()) {
            cout << "Level " << l << ": ";

            int siz = levels.size();
            while (siz--) {
                BinaryNode* it = levels.front();
                cout << it->data << ' ';

                if (it->left)
                    levels.push(it->left);
                if (it->right)
                    levels.push(it->right);

                levels.pop();
            }

            cout << endl;
            ++l;
        }
    }

    BinaryNode* min_node(BinaryNode* node) {
        BinaryNode* cur = node;
        while (cur->left)
            cur = cur->left;
        return cur;
    }
    BinaryNode* delete_node(BinaryNode* node, type target) {
        if (!node)
            return nullptr;

        if (target < node->data)
            node->left = delete_node(node->left, target); // update left
        else if (target > node->data)
            node->right = delete_node(node->right, target);
        else { // found the node to be deleted !
            BinaryNode* tmp = node;

            if (!node->left and !node->right) // case 1: no children
                node = nullptr;
            else if (!node->right) // case 2: one left child
                node = node->left;
            else if (!node->left) // case 2: one right child
                node = node->right;
            else { // case 3: 2 children => use Successor
                BinaryNode* suc = min_node(node->right);
                node->data = suc->data; // copy suc data
                node->right = delete_node(node->right, node->data); // remove the original suc
                tmp = nullptr; // don't delete me
            }

            if (tmp)
                delete tmp;
        }

        return node;
    }

public:
    void insert(type val) {
        if (!root) root = new BinaryNode(val);
        else insert_node(root, val);
    }

    bool search(type target) {
        if (root)
            return search_node(root, target);
        return false;
    }

    void print_inorder() {
        if (root)
            print_inorder_node(root);
    }
    void level_order_traversal() {
        if (root)
            level_order_trav_node();
    }

    void delete_value(type target) {
        if (root)
            root = delete_node(root, target);
    }
};


////////// test and main ////////

void test_bst() {
    BST<int> bst(10);

    // insert:
    bst.insert(7);
    bst.insert(5);
    bst.insert(13);
    bst.insert(9);

    bst.print_inorder(); cout << endl;

    // min & max:
    cout << bst.min_value() << ' ' << bst.max_value() << endl;

    // is bst:
    cout << bst.is_bst1() << endl;
    cout << bst.is_bst2() << endl;

    // build balanced bst:
    vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7};
    build_balanced_bst<int>(v)->print_inorder(); cout << endl;

    // kth smallest:
    int k = 4;
    cout << bst.kth_smallest(k) << endl;

    // lca:
    cout << bst.lca(10, 9) << endl;

    // successor:
    cout << bst.successor(7).second << ' ' << bst.successor(9).second << endl;
    cout << bst.successor(13).first << endl; // false

    // is degenerate:
    vector<char> pre;
    pre = {'c', 'a'};
    /*for (char c = 'a'; c <= 'i'; ++c)
        pre.push_back(c);*/
    if (is_degenerate(pre))
        cout << "degenerate\n";
}

void test_bst_parent() {
    /// bst with parent:
    cout << "\nbst with parent\n";
    BST_Parent pbst(3);
    pbst.insert(2);
    pbst.insert(5);
    pbst.insert(4);
    pbst.insert(1);
    pbst.insert(7);

    pbst.print_inorder(); cout << endl;
    pbst.print_parent(5);
    pbst.print_parent(3);
    pbst.print_parent(7);

    // successor:
    cout << "successors: ";
    cout << pbst.successor(4).second << ' ';
    cout << pbst.successor(1).second << ' ';
    cout << pbst.successor(3).second << ' ';
    cout << pbst.successor(7).second << ' ';
    cout << endl;
}

void test_queries_ancestors() {
    BST<int> bst(15);
    bst.insert(10);
    bst.insert(20);
    bst.insert(7);
    bst.insert(11);
    bst.insert(18);
    bst.insert(23);
    bst.insert(13);
    bst.insert(19);
    bst.insert(25);

    deque<int> q = {7, 10, 20};
    vector<int> succ, traversal;
    bst.successor_queries(q, succ, traversal);

    for (int i : succ)
        cout << i << ' ';
    cout << endl;
}

void test_deletion() {
    BST<int> bst1(10);
    bst1.insert(7);

    bst1.print_inorder(); cout << endl;

    bst1.delete_value(10);
    bst1.print_inorder(); cout << endl;

    BST<int> bst2(50);
    bst2.insert(31);
    bst2.insert(66);
    bst2.insert(16);
    bst2.insert(40);
    bst2.insert(60);
    bst2.insert(77);

    bst2.print_inorder(); cout << endl;
    bst2.delete_value(50);

    bst2.print_inorder(); cout << endl;

}

void test_bst_binarynode() {
    BinarySearchTree<int> bstBN;
    bstBN.insert(10);
    bstBN.insert(6);
    bstBN.insert(9);
    bstBN.insert(13);
    bstBN.insert(11);
    bstBN.insert(1);

    bstBN.print_inorder(); cout << endl;

    cout << bstBN.search(1) << ' ' << bstBN.search(10) << ' ' << bstBN.search(15) << endl;

    bstBN.level_order_traversal();

    // delete node:
    bstBN.delete_value(6);
    bstBN.delete_value(10);
    bstBN.delete_value(9);
    bstBN.print_inorder(); cout << endl;

    bstBN.delete_value(1);
    bstBN.delete_value(11);
    bstBN.print_inorder(); cout << endl;

    bstBN.delete_value(13);

    BinarySearchTree<int> bstBN2;
    bstBN2.insert(10);
    bstBN2.insert(5);

    bstBN2.delete_value(10);
    bstBN2.print_inorder();



}

int main() {
    //test_bst();
    //test_bst_parent();
    //test_queries_ancestors();
    //test_deletion();
    test_bst_binarynode();

    cout << "\nbye :)\n";

    return 0;
}