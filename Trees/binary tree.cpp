/// Binary Tree
// author : Saeed Ahmed
// 25 / 12 / 2022

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>

using namespace std;

template <typename type>
class BinaryTree {
    /// the common style:
    /*struct Node {
        type data;
        Node* left {};
        Node* right {};

        Node(type _data) : data(_data) { }
        ~Node() { cout << "Delete: " << data << endl; }
    };

    Node* root {};*/

    type data {};
    BinaryTree* left {};
    BinaryTree* right {};

    /// utility functions:
    bool is_leaf() { return !left and !right; }

public:
    BinaryTree(type _data) : data(_data) { }
    BinaryTree(deque<type>& preorder, deque<type>& inorder) {
        // build general binary trees
        // assume that all values are unique

        data = preorder.front(); // next root
        preorder.pop_front();

        deque<type> leftInorder, rightInorder;
        while (inorder.size() > 1) {
            if (inorder.back() != data) {
                rightInorder.push_front(inorder.back());
                inorder.pop_back();
            }
            if (inorder.front() != data) {
                leftInorder.push_back(inorder.front());
                inorder.pop_front();
            }
        }

        if (!leftInorder.empty())
            left = new BinaryTree<type>(preorder, leftInorder);
        if (!rightInorder.empty())
            right = new BinaryTree<type>(preorder, rightInorder);
    }
    BinaryTree(deque< pair<type, bool> > &preorder) {
        // build only the full BTs
        // pair of (value, is_leaf)

        pair<type, bool> root = preorder.front();
        preorder.pop_front();

        data = root.first;
        if (!root.second) {
            left = new BinaryTree(preorder);
            right = new BinaryTree(preorder);
        }
    }

    void add(vector<type> nodes, string path) {
        assert(nodes.size() == path.size());
        BinaryTree* cur = this;
        for (int i = 0; i < path.size(); ++i) {
            if (path[i] == 'L') {
                if (!cur->left)
                    cur->left = new BinaryTree(nodes[i]);
                else
                    assert(cur->left->data == nodes[i]);
                cur = cur->left;
            }
            else {
                if (!cur->right)
                    cur->right = new BinaryTree(nodes[i]);
                else
                    assert(cur->right->data == nodes[i]);
                cur = cur->right;
            }
        }
    }

    void print_inorder() {
        if (left)
            left->print_inorder();
        cout << data << ' ';
        if (right)
            right->print_inorder();
    }
    void print_preorder() {
        cout << data << ' ';
        if (left) left->print_preorder();
        if (right) right->print_preorder();
    }
    void print_preorder_complete() {
        cout << data << ' ';
        if (left) left->print_preorder();
        else cout << -1 << ' ';
        if (right) right->print_preorder();
        else cout << -1 << ' ';
    }

    /// Easy challenges:
    type tree_max() {
        static type mx {}; // to avoid re-initialization in the recursive calls

        mx = max(mx, data);
        if (left)
            left->tree_max();
        if (right)
            right->tree_max();

        return mx;
    }

    int tree_height() {
        int h = -1;
        if (left)
            h = max(h, left->tree_height());
        if (right)
            h = max(h, right->tree_height());

        return h + 1;
    }

    int count_nodes() {
        int cnt = 1;
        if (left)
            cnt += left->count_nodes();
        if (right)
            cnt += right->count_nodes();
        return cnt;
    }

    int count_leaves() {
        int l = !left and !right;
        if (left)
            l += left->count_leaves();
        if (right)
            l += right->count_leaves();

        return l;
    }

    bool is_exists(int val) {
        bool exist = data == val;
        if (!exist and left)
            exist = left->is_exists(val);
        if (!exist and right)
            exist = right->is_exists(val);
        return exist;
    }

    bool is_full() {
        bool res = is_leaf() or (left and right);
        if (res and left)
            res = left->is_perfect_formula();
        if (res and right)
            res = right->is_perfect_formula();

        return res;
    }

    bool is_perfect_recurse(int h = -1) {
        if (h == -1)
            h = tree_height();

        if (is_leaf()) return h == 0; // leaves must be in the same level

        if (!(left and right)) return false; // one child..

        return left->is_perfect_formula(h - 1) and right->is_perfect_formula(h - 1);
    }
    bool is_perfect_formula() {
        return count_nodes() == (pow(2, tree_height() + 1) - 1);
    }

    ///

    void level_order_traversal() {
        // we can do it recursively, but the queue is mush more efficient..
        queue< BinaryTree<type>* > nodesQue; nodesQue.push(this);

        int level = 0;
        while (!nodesQue.empty()) {
            int siz = nodesQue.size();
            cout << "Level " << level << ": ";
            while (siz--) {
                BinaryTree<type>* cur = nodesQue.front();
                cout << cur->data << ' ';
                if (cur->left)
                    nodesQue.push(cur->left);
                if (cur->right)
                    nodesQue.push(cur->right);
                nodesQue.pop();
            }
            ++level;
            cout << '\n';
        }
    }

    /// challenges:
    void print_nodes_level(int level) {
        if (level == 0)
            cout << data << ' ';
        else {
            if (left)
                left->print_nodes_level(level - 1);
            if (right)
                right->print_nodes_level(level - 1);
        }
    }
    void level_order_traversal_recursive() { // O(n^2) // don't know how :)
        int h = tree_height();
        // do the recursive traversal level by level:
        for (int level = 0; level <= h; ++level) {
            print_nodes_level(level);
        }
    }

    void level_order_traversal_spiral() {
        deque< BinaryTree<type>* > nodesQue; nodesQue.push_back(this);
        // allows both queue and stack styles !

        int level = 0;
        bool forwardLevel = true;
        while (!nodesQue.empty()) {
            int siz = nodesQue.size();
            cout << "Level " << level << ": ";
            while (siz--) {
                BinaryTree<type>* cur;

                if (forwardLevel) { // queue style
                    cur = nodesQue.front();
                    nodesQue.pop_front();

                    if (cur->left)
                        nodesQue.push_back(cur->left);
                    if (cur->right)
                        nodesQue.push_back(cur->right);
                }
                else { // stack style
                    cur = nodesQue.back();
                    nodesQue.pop_back();

                    if (cur->right)
                        nodesQue.push_front(cur->right);
                    if (cur->left)
                        nodesQue.push_front(cur->left);
                }

                cout << cur->data << ' ';
            }
            cout << '\n';
            ++level;
            forwardLevel = !forwardLevel;
        }
    }

    bool is_complete() {
        // all levels must be complete, except possibly the last one
        // but must be filled from the left

        // if reached a missing node, no more nodes allowed
        // in this level or even in the next levels
        bool noMoreAllowed = false;

        queue< BinaryTree<type>* > nodesQue; nodesQue.push(this);

        while (!nodesQue.empty()) {
            int siz = nodesQue.size();
            while (siz--) {
                BinaryTree<type>* cur = nodesQue.front();
                nodesQue.pop();

                if (cur->left) {
                    if (noMoreAllowed) return false;
                    nodesQue.push(cur->left);
                }
                else noMoreAllowed = true;

                if (cur->right) {
                    if (noMoreAllowed) return false;
                    nodesQue.push(cur->right);
                }
                else noMoreAllowed = true;
            }
        }

        return true;
    }

    /// parenthesize and canonicalize:
    string parenthesize_canonical() {
        // goal: providing a unique representation of the tree in one data structure (e.g. array, string)
        string repr = "(" + to_string(data);

        vector<string> v;
        if (left)
            v.push_back(left->parenthesize_canonical());
        else v.push_back("()"); // indicating null

        if (right)
            v.push_back(right->parenthesize_canonical());
        else v.push_back("()");

        // canonicalizing:
        sort(v.begin(), v.end());
        for (int i = 0; i < v.size(); ++i) {
            repr += v[i];
        }

        repr += ")";
        return repr;
    }

    /// challenges:
    bool is_mirror(BinaryTree<type>* first, BinaryTree<type>* second) { // O(n)
        if (!first and !second) return true;
        else if (!first and second
                or !second and first)
            return false;

        return first->data == second->data
            and is_mirror(first->left, second->right)
            and is_mirror(first->right, second->left);
    }
    bool is_symmetric() {
        return is_mirror(left, right);
    }

    string parenthesize(bool rightFirst = false) { // O(n)
        string repr = "(" + to_string(data);
        if (rightFirst) {
            if (right)
                repr += right->parenthesize(rightFirst);
            else repr += "()";

            if (left)
                repr += left->parenthesize(rightFirst);
            else repr += "()";
        }
        else {
            if (left)
                repr += left->parenthesize();
            else repr += "()";

            if (right)
                repr += right->parenthesize();
            else repr += "()";
        }

        repr += ")";
        return repr;
    }
    bool is_symmetric2() {
        if (is_leaf()) return true;
        if (!left and right or left and !right) return false; // one subtree only

        return left->parenthesize() == right->parenthesize(true);
    }

    bool is_flip_equiv(BinaryTree<type>* other) {
        return parenthesize_canonical() == other->parenthesize_canonical();
    }

    string parenthesize_duplicates(map<string, int> &duplicates) {
        string repr = "(" + to_string(data);
        if (left)
            repr += left->parenthesize_duplicates(duplicates);
        else repr += "()";

        if (right)
            repr += right->parenthesize_duplicates(duplicates);
        else repr += "()";

        repr += ")";
        if (!is_leaf())
            duplicates[repr]++;

        return repr;
    }
    void duplicate_subtrees() {
        map<string, int> duplicates;
        parenthesize_duplicates(duplicates);
        for(auto& it : duplicates) {
            if (it.second > 1)
                cout << it.first << endl;
        }
    }
};


////////// test & main //////////

void test() {
    BinaryTree<int> tree1(2);
    tree1.add({3, 2}, "LL");
    tree1.add({13, 7}, "RL");
    tree1.add({13, 8}, "RR");
    tree1.add({13, 8, 3, 2}, "RRLL");

    tree1.duplicate_subtrees();

    //cout << tree1.parenthesize_canonical() << endl;

    //tree1.level_order_traversal(); cout << endl;

    BinaryTree<int> tree2(1);
    tree2.add({2, 4, 7}, "LLL");
    tree2.add({2, 4, 8}, "LLR");
    tree2.add({2, 5, 9}, "LRR");
    tree2.add({3, 6, 10}, "RRL");

    tree2.print_preorder();
    cout << endl;

    //tree2.level_order_traversal_spiral(); cout << endl;

    BinaryTree<int> perfect(2);
    perfect.add({3, 5}, "LL");
    perfect.add({3, 6}, "LR");
    perfect.add({13, 7}, "RL");
    perfect.add({13, 8}, "RR");

    BinaryTree<int> complete(2);
    complete.add({3}, "R");
    complete.add({13, 7}, "LL");
    complete.add({13, 8}, "LR");
    complete.add({3, 4}, "RL");


}

int main() {
    test();

    return 0;
}