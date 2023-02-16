#include <bits/stdc++.h>
using namespace std;

/// AVL Tree
// is a Balanced BST (BBST)
// very important for interviews

template <typename type>
class AVLTree {
    struct BinaryNode {
        type data {};
        int count {1}; // for count inversions
        int height {};
        BinaryNode *left {};
        BinaryNode *right{};

        BinaryNode(type data) : data(data) { }

        int child_height(BinaryNode* node) {
            if (!node)
                return -1;
            return node->height;
        }
        int child_count(BinaryNode* node) {
            if (!node)
                return 0;
            return node->count;
        }
        void update() { // call after the insert func
            height = 1 + max(child_height(left), child_height(right));
            count = 1 + child_count(left) + child_count(right);
        }
        int balance_factor() {
            return child_height(left) - child_height(right);
        }
    };
    BinaryNode* root {};

    BinaryNode* right_rotation(BinaryNode* Q) {
        BinaryNode* P = Q->left;
        Q->left = P->right;
        P->right = Q;

        // notice the order of calling ;)
        // update the subtree then update the root
        Q->update();
        P->update();
        return P;
    }
    BinaryNode* left_rotation(BinaryNode* P) {
        BinaryNode* Q = P->right;
        P->right = Q->left;
        Q->left = P;

        // notice the order of calling ;)
        // update the subtree then update the root
        P->update();
        Q->update();
        return Q;
    }

    // Fix:
    BinaryNode* balance(BinaryNode* node) { // O(1)
        if (node->balance_factor() == 2) { // Left
            if (node->left->balance_factor() == -1) // left right?
                node->left = left_rotation(node->left); // to Left Left

            node = right_rotation(node); // Balanced Left Left
        }
        else if (node->balance_factor() == -2) {
            if (node->right->balance_factor() == 1) // right left?
                node->right = right_rotation(node->right);

            node = left_rotation(node);
        }

        return node;
    }

    BinaryNode* insert_node(type target, BinaryNode* node) {
        if (target < node->data) {
            if (!node->left)
                node->left = new BinaryNode(target);
            else // update the left, because it may be imbalanced
                node->left = insert_node(target, node->left);
        }
        else if (target > node->data) {
            if (!node->right)
                node->right = new BinaryNode(target);
            else
                node->right = insert_node(target, node->right);
        }

        node->update();
        return balance(node);
    }
    BinaryNode* insert_node(type target, BinaryNode* node, int& inv) {
        // insert + count inversions

        if (target < node->data) {
            inv += (1 + node->child_count(node->right));

            if (!node->left)
                node->left = new BinaryNode(target);
            else // update the left, because it may be imbalanced
                node->left = insert_node(target, node->left, inv);
        }
        else if (target > node->data) {
            if (!node->right)
                node->right = new BinaryNode(target);
            else
                node->right = insert_node(target, node->right, inv);
        }

        node->update();
        return balance(node);
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

        if (node) {
            node->update();
            node = balance(node);
        }
        return node;
    }

    void print_inorder_node(BinaryNode* node) {
        if (node->left)
            print_inorder_node(node->left);
        cout << node->data << ' ';
        if (node->right)
            print_inorder_node(node->right);
    }
    void print_preorder_node(BinaryNode* node) {
        cout << node->data << ' ';
        if (node->left)
            print_preorder_node(node->left);
        if (node->right)
            print_preorder_node(node->right);
    }

    BinaryNode* lower_bound_node(type target, BinaryNode* node) {
        if (!node)
            return nullptr;
        if (node->data >= target) { // can be my ans
            // search for a tighter node:
            BinaryNode* ans = lower_bound_node(target, node->left);
            if (ans)
                return ans;
            return node;
        }
        
        return lower_bound_node(target, node->right);
    }
    BinaryNode* upper_bound_node(type target, BinaryNode* node) {
        if (!node)
            return nullptr;
        if (node->data > target) { // can be my ans
            // search for a tighter node:
            BinaryNode* ans = upper_bound_node(target, node->left);
            if (ans)
                return ans;
            return node;
        }

        return upper_bound_node(target, node->right);
    }

    bool search_word_node(string word, BinaryNode* node) { // O(log n)
        if (word == node->data)
            return true;
        else if (word < node->data)
            return node->left and search_word_node(word, node->left);
        else
            return node->right and search_word_node(word, node->right);
    }
    bool search_prefix_node(string prefix, BinaryNode* node) { // O(log n)
        string sub = node->data.substr(0, min(node->data.size(), prefix.size()));

        if (prefix == sub)
            return true;
        else if (prefix < sub)
            return node->left and search_prefix_node(prefix, node->left);
        else
            return node->right and search_prefix_node(prefix, node->right);
    }

public:
    bool empty() { return !root; }

    void insert(type target) {
        if (!root)
            root = new BinaryNode(target);
        else root = insert_node(target, root);
    }

    void delete_value(type target) {
        if (root)
            delete_node(root, target);
    }

    void print_inorder() {
        if (root)
            print_inorder_node(root);
    }
    void print_preorder() {
        if (root)
            print_preorder_node(root);
    }

    pair<bool, int> lower_bound(type target) {
        BinaryNode *node = lower_bound_node(target, root);
        if (!node)
            return {false, -1};
        return {true, node->data};
    }
    pair<bool, int> upper_bound(type target) {
        BinaryNode *node = upper_bound_node(target, root);
        if (!node)
            return {false, -1};
        return {true, node->data};
    }

    int count_inversions(vector<type>& v) {
        // must the tree to be empty
        if (!empty())
            return -1;

        int inv = 0;
        // {3, 5, 9, 1, 0, 2, 6, 10, 7, 4, 8}
        for (int i : v) {
            if (!root)
                root = new BinaryNode(i);
            else root = insert_node(i, root, inv);
        }
        return inv;
    }

    bool word_exist(string word) {
        if (root)
            return search_word_node(word, root);
        return false;
    }

    bool prefix_exist(string prefix) {
        if (root)
            return search_prefix_node(prefix, root);
        return false;
    }
};


/////////////////////////////////////////////
/// challenge: Priority Queue based on AVL tree
template <typename type>
class PriorityQueue {
    struct BinaryNode {
        int priority {};
        vector<type> taskIds; // all tasks have same priority
        int height {};
        BinaryNode *left {};
        BinaryNode *right{};

        BinaryNode(int priority) : priority(priority) { }

        void add(type item) {
            taskIds.push_back(item);
        }
        type get() {
            type ret = taskIds.back();
            taskIds.pop_back();
            return ret;
        }

        int child_height(BinaryNode* node) {
            if (!node)
                return -1;
            return node->height;
        }
        void update() { // call after the insert func
            height = 1 + max(child_height(left), child_height(right));
        }
        int balance_factor() {
            return child_height(left) - child_height(right);
        }
    };
    BinaryNode* root {};

    BinaryNode* min_node(BinaryNode* node) {
        while (node and node->left)
            node = node->left;
        return node;
    }
    BinaryNode* max_node(BinaryNode* node) { // O(log n)
        while (node and node->right)
            node = node->right;
        return node;
    }

    BinaryNode* right_rotation(BinaryNode* Q) {
        BinaryNode* P = Q->left;
        Q->left = P->right;
        P->right = Q;

        // notice the order of calling ;)
        // update the subtree then update the root
        Q->update();
        P->update();
        return P;
    }
    BinaryNode* left_rotation(BinaryNode* P) {
        BinaryNode* Q = P->right;
        P->right = Q->left;
        Q->left = P;

        // notice the order of calling ;)
        // update the subtree then update the root
        P->update();
        Q->update();
        return Q;
    }

    BinaryNode* balance(BinaryNode* node) { // O(1)
        if (node->balance_factor() == 2) { // Left
            if (node->left->balance_factor() == -1) // left right?
                node->left = left_rotation(node->left); // to Left Left

            node = right_rotation(node); // Balanced Left Left
        }
        else if (node->balance_factor() == -2) {
            if (node->right->balance_factor() == 1) // right left?
                node->right = right_rotation(node->right);

            node = left_rotation(node);
        }

        return node;
    }

    BinaryNode* insert_node(const type target, const int priority, BinaryNode* node) {
        if (priority < node->priority) {
            if (!node->left)
                node->left = new BinaryNode(priority), node->left->add(target);
            else // update the left, because it may be imbalanced
                node->left = insert_node(target, priority, node->left);
        }
        else if (priority > node->priority) {
            if (!node->right)
                node->right = new BinaryNode(priority), node->right->add(target);
            else
                node->right = insert_node(target, priority, node->right);
        }
        else { // same priority exist before
            node->add(target);
        }

        node->update();
        return balance(node);
    }

    BinaryNode* delete_node(BinaryNode* node, int priority) {
        if (!node)
            return nullptr;

        if (priority < node->priority)
            node->left = delete_node(node->left, priority); // update left
        else if (priority > node->priority)
            node->right = delete_node(node->right, priority);
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
                node->priority = suc->priority; // copy suc priority
                node->right = delete_node(node->right, node->priority); // remove the original suc
                tmp = nullptr; // don't delete me
            }

            if (tmp)
                delete tmp;
        }

        if (node) {
            node->update();
            node = balance(node);
        }
        return node;
    }

public:
    bool empty() { return !root; }

    void enqueue(const type target, const int priority) {
        if (!root)
            root = new BinaryNode(priority), root->add(target);
        else root = insert_node(target, priority, root);
    }

    type dequeue() {
        assert(!empty());

        BinaryNode* mx = max_node(root);
        type ret = mx->get();
        if (mx->taskIds.empty())
            root = delete_node(root, mx->priority);

        return ret;
    }
};

//////////////////////////////////////

/// min number of nodes for AVL height:
int avl_nodes_rec(int height) {
    if (height == 0)
        return 1;
    else if (height == 1)
        return 2;

    return 1 + avl_nodes_rec(height - 1) + avl_nodes_rec(height - 2);

    // what if BF = k?
    // ans = 1 + f(height - 1) + f(height - k - 1)
}
int avl_nodes_iter(int height) {
    if (height == 0)
        return 1;
    else if (height == 1)
        return 2;

    int a = 1, b = 2, c;
    for (int i = 2; i <= height; ++i)
        c = 1 + a + b, a = b, b = c;

    return c;
}

//////////// test and main ////////////

void test_avl() {
    AVLTree<int> avl;


    //vector<int> v = {3, 5, 9, 1, 0, 2, 6, 10, 7, 4, 8}; // 20
    //vector<int> v1 = {10, 5, 8, 2, 12, 6}; // 8
    vector<int> v2 = {5, 4, 3, 2, 1}; // 10
    cout << avl.count_inversions(v2) << endl;

    avl.print_inorder(); cout << endl;
    cout << avl.upper_bound(-1).second << endl;
    cout << avl.upper_bound(4).second << endl;
    cout << avl.upper_bound(11).second << endl;
}

void test_priority_queue() {
    PriorityQueue<int> tasks;
    tasks.enqueue(1131, 1);
    tasks.enqueue(3111, 3);
    tasks.enqueue(2211, 2);
    tasks.enqueue(3161, 3);

    cout << tasks.dequeue() << endl;
    cout << tasks.dequeue() << endl;
    cout << tasks.dequeue() << endl;

}

void test_min_nodes() {
    for (int i = 1; i <= 6; ++i) {
        cout << avl_nodes_iter(i) << ' ';
    }
    cout << endl;
}

void test_avl_string() {
    AVLTree<string> avlString;
    avlString.insert("abcd");
    avlString.insert("xyz");

    cout << avlString.word_exist("abcd") << endl; // 1
    cout << avlString.word_exist("a") << endl; // 0
    cout << avlString.prefix_exist("a") << " " << avlString.prefix_exist("xy") << endl; // 1 1

    avlString.insert("ba");
    cout << avlString.prefix_exist("bcd") << endl; // 0
}

int main()
{
    //test_avl();

    //test_priority_queue();

    //test_min_nodes();

    test_avl_string();

    return 0;
}