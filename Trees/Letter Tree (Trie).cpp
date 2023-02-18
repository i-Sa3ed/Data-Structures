#include <iostream>
#include <map>
#include <algorithm> // reverse
#include <vector>

using namespace std;

class Trie {
    map<char, Trie*> children; // more efficient than array (in memory space)
    bool isLeaf {}; // if true => complete word

public:
    Trie() {
    }

    // coding iteratively is better than recursively, because of the auxiliary space of recursive code
    void insert(string word) {
        Trie* cur = this;
        for (char & i : word) {
            if (cur->children.find(i) == cur->children.end())
                cur->children[i] = new Trie();
            cur = cur->children[i];
        }
        cur->isLeaf = true; // complete word
    }

    bool word_exist(const string& word) { // O(L) [L = length of words]
        Trie* cur = this;
        for (char i : word) {
            if (cur->children.find(i) == cur->children.end())
                return false;
            cur = cur->children[i];
        }

        return cur->isLeaf; // the difference between complete word and prefix
    }

    bool prefix_exist(const string& prefix) { // O(L)
        Trie* cur = this;
        for (char i : prefix) {
            if (cur->children.find(i) == cur->children.end())
                return false;
            cur = cur->children[i];
        }

        return true;
    }

    string first_word_prefix(const string &word) { // O(L)
        Trie* cur = this;
        string prefix;

        for (char c : word) {
            if (cur->children.find(c) != cur->children.end()) {
                prefix += c;
                cur = cur->children[c];

                if (cur->isLeaf)
                    return prefix;
            }
            else break;
        }

        return word;
    }

    ////

    void debug() {
        Trie* cur = this;
        cout << cur->isLeaf << ' ';

        for (auto it : cur->children)
            cout << it.first << endl, it.second->debug();
    }
};

///////////////////////////////////////

/// challenge: Suffix Trie
// same idea but just reverse the input

class SuffixTrie {
    map<char, SuffixTrie*> children; // more efficient than array (in memory space)
    bool isLeaf {}; // if true => complete word

public:
    SuffixTrie() {
    }

    void insert(string word) {
        reverse(word.begin(), word.end());

        SuffixTrie* cur = this;
        for (char & i : word) {
            if (cur->children.find(i) == cur->children.end())
                cur->children[i] = new SuffixTrie();
            cur = cur->children[i];
        }
        cur->isLeaf = true; // complete word
    }

    bool suffix_exist(string word) { // O(L) [L = length of words]
        reverse(word.begin(), word.end());

        SuffixTrie* cur = this;
        for (char i : word) {
            if (cur->children.find(i) == cur->children.end())
                return false;
            cur = cur->children[i];
        }

        return true;
    }
};

///////////////////////////////////////

/// challenge: OS paths
// the only change is to use a map of "string" instead of "char"

class StringTrie {
    map<string, StringTrie*> children;
    bool isLeaf {};

public:
    StringTrie() {
    }

    void insert(vector<string>& path) {
        StringTrie* cur = this;
        for (string& dir : path) {
            if (cur->children.find(dir) == cur->children.end())
                cur->children[dir] = new StringTrie();
            cur = cur->children[dir];
        }
        cur->isLeaf = true; // end of path
    }
    bool subpath_exist(vector<string>& path) {
        StringTrie* cur = this;

        for (string& dir : path) {
            if (cur->children.find(dir) == cur->children.end())
                return false;
            cur = cur->children[dir];
        }

        return true;
    }
};

/////////////////// test and main ///////////////////

void test_trie() {
    Trie root;

    root.insert("abcd");
    root.insert("xyz");
    root.insert("abf");
    root.insert("xn");
    root.insert("ab");
    root.insert("bcd");

    cout << root.first_word_prefix("abcde") << endl; // ab
    cout << root.first_word_prefix("xyzsfh") << endl; // xyz
    cout << root.first_word_prefix("msh") << endl; // msh
}

void test_suffix_trie() {
    SuffixTrie suffixTrie;
    suffixTrie.insert("problem");

    cout << suffixTrie.suffix_exist("blem") << endl; // 1
    cout << suffixTrie.suffix_exist("m") << endl; // 1
    cout << suffixTrie.suffix_exist("ble") << endl; // 0
    cout << suffixTrie.suffix_exist("proble") << endl; // 0
    cout << suffixTrie.suffix_exist("problem") << endl; // 1
}

void test_os_paths() {
    StringTrie root;
    vector<string> path = {"user", "saeed", "tmp"};
    root.insert(path);

    //root.debug();

    cout << root.subpath_exist(path) << endl; // 1

    path = {"usersaeed", "tmp"};
    cout << root.subpath_exist(path) << endl; // 0

    path = {"user", "saeed"};
    cout << root.subpath_exist(path) << endl; // 1

    path = {"saeed", "tmp"};
    cout << root.subpath_exist(path) << endl; // 0

    path = {"use", "rsaeed", "tmp"};
    cout << root.subpath_exist(path) << endl; // 0

}

int main() {
    //test_trie();

    //test_suffix_trie();

    test_os_paths();

    return 0;
}