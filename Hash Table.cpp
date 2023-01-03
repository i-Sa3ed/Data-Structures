//
// Created by Saeed on 1/3/2023.
//

#include <iostream>
#include <vector>

using namespace std;

// hash function: convert the input into a fixed-size value
int hash_num(int value, int n) {
    return value % n;
    // hashed the value in range [0, n-1]
}
// another example of hashing functions: string hashing
int hash_string(string str, int n) {
    long long nn = n, sum = 0; // to avoid the overflow
    for (int i = 0; i < str.size(); ++i) {
        sum = (sum * 26 + (str[i] - 'a')) % nn;
    }

    return sum;
}

// collision: the situation in which the hash function produces the same output for different input
// the less collision occurs, the better hash function

/*
 * 2 ways to handle the collision:
 * 1- chaining: array of linked list/AVL tree/vector/etc.. (linked list for the values with the same key)
 * 2- probing: creates only one single array, in case of collision: search linearly for an empty place
 */

struct PhoneEntry {
    const static int INTERNAL_LIMIT = 1e9;
    string name;
    string phoneNumber;

    PhoneEntry(string name, string number) : name(name), phoneNumber(number) { }

    int hash() {
        return hash_string(name, INTERNAL_LIMIT);
    }

    void print() {
        cout << '(' << name << ", " << phoneNumber << ")\t";
    }
};

/// chaining implementation:
class PhoneHashTable {
    int tableSize;
    vector< vector<PhoneEntry> > table;
    // we can use another data structures (e.g. list< vector<phoneEntry> >)

public:
    PhoneHashTable(int tableSize) :
        tableSize(tableSize) {
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

// load factor = (total number of elements / table size)
// important for knowing the distribution of the elements

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
                delete table[idx];
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


int main() {
    /// std::hash => a built-in hashing function
    cout << hash<string>{} ("saeed"); // 16186113463370327406

    // Note: don't use doubles as keys in the hashing.. because they are approximate
}