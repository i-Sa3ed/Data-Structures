#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_set>
#include <algorithm> // sort

using namespace std;

/// Hashing Functions:

// hash function: convert the input into a fixed-size value
int hash_num(int value, int n = 65407) {
    return value % n;
    // hashed the value in range [0, n-1]
}

// another example of hashing functions: string hashing
int hash_string(string str, int n = 65407) {
    int base = 2 * 26 + 10; // 2 * alp + digits
    long long nn = n, sum = 0;

    for (int i = 0; i < str.size(); ++i) {
        int value;
        if (islower(str[i]))
            value = str[i] - 'a'; // [0, 25]
        else if (isupper(str[i]))
            value = 26 + str[i] - 'A'; // [26, 51]
        else if (isdigit(str[i]))
            value = 2*26 + str[i] - '0'; // [52, 61]
        else
            assert(false); // invalid input

        sum = sum * base + value;
        sum %= nn; // minimize overflow
    }

    return sum;
}

int hash_string_folding(const string& str, int n = 65407) {
    long long nn = n, sum = 0;
    int siz = str.size();
    for (int i = 0; i < siz; i += 4) {
        int subSize = min(4, siz - i);
        sum += hash_string(str.substr(i, subSize), n);
        sum %= nn;
    }

    return sum;
}

// collision: the situation in which the hash function produces the same output for different input
// the less collision occurs, the better hash function

/*
 * 2 ways to handle the collision:
 * 1- chaining: array of linked list/AVL tree/vector/etc.. (linked list for the inputs with the same hashing value)
 * 2- probing: creates only one single array, in case of collision: search linearly for an empty place
 */

struct PhoneEntry {
    const static int INTERNAL_LIMIT = 2147483647;
    string name; // key
    string phoneNumber; // data

    PhoneEntry(string name, string number) : name(name), phoneNumber(number) { }

    int hash() {
        return hash_string(name, INTERNAL_LIMIT);
    }

    void print() {
        cout << '(' << name << ", " << phoneNumber << ")\t";
    }
};

/// chaining implementation:
// load factor = (total number of elements / table size)
// important for knowing the distribution of the elements

class PhoneHashTable {
    int tableSize;
    int added {};
    double limitLoadFactor;

    vector< vector<PhoneEntry> > table;
    // we can use another data structures (e.g. list< vector<phoneEntry> >)

    void rehashing() {
        PhoneHashTable newTable(tableSize * 2, limitLoadFactor); // create new table of double size

        // copy data to it:
        for (int i = 0; i < tableSize; ++i) {
            for (PhoneEntry& phone : table[i])
                newTable.put(phone);
        }

        // retrieve & delete
        tableSize *= 2;
        table = newTable.table;
    }

public:
    PhoneHashTable(int tableSize = 10, double limitLoadFactor = 0.75) :
        tableSize(tableSize), limitLoadFactor(limitLoadFactor) {
        table.resize(tableSize);
    }

    void put(PhoneEntry phone) {
        int idx = phone.hash() % tableSize; // reduce it to fit my table length
        for (int i = 0; i < table[idx].size(); ++i) {
            if (phone.name == table[idx][i].name) {
                table[idx][i] = phone; // exist => update
                return;
            }
        }

        table[idx].push_back(phone); // not exist => add it
        ++added;

        if (double(added) / tableSize > limitLoadFactor) // now we are above the limit factor
            rehashing();
    }

    bool remove(PhoneEntry phone) {
        int idx = phone.hash() % tableSize;
        for (int i = 0; i < table[idx].size(); ++i) {
            if (phone.name == table[idx][i].name) {
                swap(table[idx][i], table[idx].back());
                table[idx].pop_back();
                return true; // deleted
            }
        }

        return false; // not found
    }

    bool get(PhoneEntry& phone) {
        // if found, update it

        int idx = phone.hash() % tableSize;
        for (int i = 0; i < table[idx].size(); ++i) {
            if (phone.name == table[idx][i].name) {
                phone = table[idx][i];
                return true; // updated
            }
        }

        return false; // not found
    }

    void print_all() {
        for (int hash = 0; hash < tableSize; ++hash) {
            if (table[hash].empty())
                continue;

            cout << "Hash " << hash << ": ";
            for (int i = 0; i < table[hash].size(); ++i) {
                table[hash][i].print();
            }
            cout << endl;
        }
    }
};

////////////////////////////////////////////////////////////

/// Challenge: implement chaining using array of linked-list
class PhoneHashTableLinked {
    struct LinkedHashEntry {
        PhoneEntry item;
        LinkedHashEntry* next {};

        LinkedHashEntry(PhoneEntry& phone) : item(phone) { }

        void print_list() {
            item.print();
            if (next)
                next->print_list();
        }
    };

    int tableSize;
    LinkedHashEntry **table {};

public:
    PhoneHashTableLinked(int size) : tableSize(size) {
        table = new LinkedHashEntry*[tableSize] {};
    }
    ~PhoneHashTableLinked() {
        for (int i = 0; i < tableSize; ++i) {
            delete [] table[i];
        }
        delete [] table;
    }

     void put(PhoneEntry phone) {
        int idx = phone.hash() % tableSize;
        if (!table[idx]) {
            table[idx] = new LinkedHashEntry(phone);
            return;
        }

        LinkedHashEntry* cur = table[idx];
        while (true) {
            if (cur->item.name == phone.name) {
                // exist => update and return
                cur->item = phone;
                return;
            }
            if (!cur->next) // reach the tail
                break;

            cur = cur->next;
        }

        // not exist => add it
        cur->next = new LinkedHashEntry(phone);
    }

    void print_all() {
        cout << "\n\n*************************\n";
        for (int i = 0; i < tableSize; ++i) {
            if (!table[i])
                continue;

            cout << "Hash " << i << ": ";
            table[i]->print_list();
            cout << endl;
        }
    }
};

////////////////////////////////////////////////////////////

/// Probing:
// the deletion is a bit tricky, consider marking the deleted cell as DELETED not EMPTY
/* Probing techniques:
 * 1- linear probing: index = (init_hash + i) % table size
 * 2- quadratic probing: index = (init_hash + i*i) % table size
 * 3- double hashing (use 2 independent hash functions) : index = (init_hash1 + init_hash2 * i) % table size
 */

/// probing implementation:
class ProbingPhoneHashTable {
    int tableSize;
    vector<PhoneEntry*> table;
    PhoneEntry* deleted = new PhoneEntry("", ""); // to mark the deleted cells

public:
    ProbingPhoneHashTable(int size) : tableSize(size) {
        table.resize(tableSize);
    }

    bool put(PhoneEntry phone) {
        int idx = phone.hash() % tableSize;

        // we can loop at most "tableSize" steps..
        for (int step = 0; step < tableSize; ++step) {
            if (table[idx] == deleted or !table[idx]) { // empty cell => add it
                table[idx] = new PhoneEntry(phone.name,  phone.phoneNumber);
                return true;
            }
            else if (table[idx]->name == phone.name) {
                table[idx]->phoneNumber = phone.phoneNumber; // update
                return true;
            }

            idx = (idx + 1) % tableSize; // next
        }

        return false; // array is full, couldn't add !
    }
    bool remove(PhoneEntry phone) {
        int idx = phone.hash() % tableSize;

        for (int step = 0; step < tableSize; ++step) {
            if (!table[idx]) // end of the consecutive block
                break;
            if (table[idx] != deleted and table[idx]->name == phone.name) {
                delete table[idx]; // destroy the allocated data
                table[idx] = deleted;
                return true;
            }

            idx = (idx + 1) % tableSize; // next
        }

        return false; // not found
    }
    bool get(PhoneEntry& phone) {
        int idx = phone.hash() % tableSize;

        for (int step = 0; step < tableSize; ++step) {
            if (!table[idx]) // end of the consecutive block
                break;
            if (table[idx] != deleted and table[idx]->name == phone.name) {
                phone.phoneNumber = table[idx]->phoneNumber; // update the argument
                return true;
            }

            idx = (idx + 1) % tableSize; // next
        }

        return false; // not found
    }

    void print_all() {
        for (int hash = 0; hash < tableSize; ++hash) {
            cout << hash << " ";
            if (!table[hash])
                cout << " E ";
            else if (table[hash] == deleted)
                cout << " X ";
            else
                table[hash]->print();
            cout << '\n';
        }
        cout << "*******************\n";
    }
};

class QuadraticProbingPhoneTable {
    int tableSize;
    vector<PhoneEntry*> table;
    PhoneEntry* deleted = new PhoneEntry("", ""); // to mark the deleted cells

    void rehash() {
        QuadraticProbingPhoneTable newTable(2 * tableSize);
        for (int i = 0; i < tableSize; ++i) {
            if (table[i] and table[i] != deleted)
                newTable.put(*table[i]);
        }

        tableSize *= 2;
        table = newTable.table;
    }

public:
    QuadraticProbingPhoneTable(int size) : tableSize(size) {
        table.resize(tableSize);
    }

    void put(PhoneEntry& phone) {
        int idx = phone.hash() % tableSize;
        int initialIdx = idx; // use it to stop looping

        int i = 1;
        do {
            if (table[idx] == deleted or !table[idx]) {
                table[idx] = new PhoneEntry(phone.name, phone.phoneNumber);
                return;
            }
            else if (table[idx]->name == phone.name) {
                table[idx]->phoneNumber = phone.phoneNumber; // update
                return;
            }

            idx = (initialIdx + i*i); // quadratic probing
            ++i;
        } while (idx != initialIdx);

        // we stopped when we cycled back to the initial index
        // rehash, then try again
        rehash();
        put(phone);
    }
};

////////////////////////////////////////////////////////////

/// String Challenges:

int count_unique_substrings(const string& str, unordered_set<string>& st) {
    // O(L^3)
    // Try all possible substrings (L^2) and insert in hashset
    // Hashing a string is typically O(L)

    for (int start = 0; start < str.size(); ++start) {
        for (int end = start; end < str.size(); ++end) {
            st.insert(str.substr(start, end - start + 1));
        }
    }
    return st.size();

    // Notice that we can solve it more effectively using Trie
    // in O(L^2) only
}

int count_substrings_match(const string& str1, const string& str2) { // O(L^3)
    // hash first string:
    unordered_set<string> unique1;
    count_unique_substrings(str1, unique1);

    // hash second string:
    unordered_set<string> unique2;
    count_unique_substrings(str2, unique2);

    // obtain intersection:
    int ans = 0;
    for (auto& it : unique1)
        ans += unique2.count(it);

    return ans;
}

int count_anagram_substrings(const string& str) { // O(L^3 logL)
    // to obtain anagrams => sort each substr before hashing..

    unordered_set<string> anagrams;
    for (int start = 0; start < str.size(); ++start) {
        for (int end = start; end < str.size(); ++end) {
            string sub = str.substr(start, end - start + 1);
            sort(sub.begin(), sub.end());
            anagrams.insert(sub);
        }
    }
    return anagrams.size();

    // can be implemented in O(L^3) only, by representing each string as a freq of letters ;)
}

////////////////////////////////////////////////////////////

////////////// test and main //////////////

vector<string> names = {"saeed", "ahmed", "ahmed", "mohammed", "Ahmed", "mona"};
vector<string> numbers = {"01060", "01015","015890" ,"012159", "01015", "011903"};

void test_hashing_functions() {
    string s = "aabcdefgAxT334gfg";
    cout << hash_string_folding(s, 1000) << endl;
}

void test_phone_table_chaining() {
    PhoneHashTable phoneTable(10, 0.45);

    PhoneEntry phone("saeed", "01060");
    phoneTable.put(phone);

    phone = PhoneEntry("ahmed", "01015");
    phoneTable.put(phone);

    phone = PhoneEntry("mohammed", "012159");
    phoneTable.put(phone);

    phone = PhoneEntry("Ahmed", "01015");
    phoneTable.put(phone);

    phone = PhoneEntry("mona", "011903");
    phoneTable.put(phone); // here we are above the limit factor

}

void test_phone_table_using_linkedList() {
    PhoneHashTableLinked table(3);

    table.put(PhoneEntry("mostafa", "604-401-120"));
    table.put(PhoneEntry("mostafa", "604-401-777"));	// update
    table.put(PhoneEntry("ali", "604-401-343"));
    table.put(PhoneEntry("ziad", "604-401-17"));
    table.put(PhoneEntry("hany", "604-401-758"));
    table.put(PhoneEntry("belal", "604-401-550"));
    table.put(PhoneEntry("john", "604-401-223"));

    table.print_all();
}

void test_string_challenges() {
    string str1 = "aaab";
    string str2 = "aaab";

    cout << count_substrings_match(str1, str2) << endl;

    string ang = "aaaaa";
    cout << count_anagram_substrings(ang) << endl;
}

int main() {
    //test_hashing_functions();

    //test_phone_table_chaining();

    //test_phone_table_using_linkedList();

    test_string_challenges();

    /// std::hash => a built-in hashing function
    cout << hash<string>{} ("saeed"); // 16186113463370327406

    // Note: don't use doubles as keys in the hashing.. because they are approximate
}