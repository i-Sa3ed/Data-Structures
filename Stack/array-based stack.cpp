#include <bits/stdc++.h>
using namespace std;

template <typename T> class Stack {
    int size {};
    int topidx {};
    T* array {};
public:
    Stack(int size) : size(size), topidx(-1) {
        array = new T[size];
    }
    ~Stack() {
        delete[] array;
    }

    bool is_full() {
        return (topidx == size -1);
    }
    bool is_empty() {
        return (topidx == -1);
    }

    void push(T item) {
        assert(!is_full());
        array[++topidx] = item;
    }
    T pop() {
        assert(!is_empty());
        return array[topidx--];
    }
    T top() {
        assert(!is_empty());
        return array[topidx];
    }

    void display() {
        for(int i = topidx; i >= 0; --i)
            cout << array[i] << ' ';
        cout << endl;
    }
};

class stackOf2 {
    int top1 {};
    int top2 {};
    int size {};
    int* array {};

public:
    stackOf2(int size) : size(size), top1(-1), top2(size) {
        array = new int[size];
    }
    ~stackOf2() {
        delete[] array;
    }

    bool is_full() {
        return top2 == top1 + 1;
    }
    bool is_empty(int id) {
        if (id == 1) return top1 == -1;
        else return top2 == size;
    }

    void push(int id, int x) {
        assert(!is_full());
        assert(id == 1 or id == 2);

        if (id == 1) array[++top1] = x;
        else array[--top2] = x;
    }
    int pop(int id) {
        assert(id == 1 or id == 2);
        assert(!is_empty(id));

        if (id == 1) {
            return array[top1--];
        }
        else {
            return array[top2++];
        }
    }
    int top(int id) {
        assert(id == 1 or id == 2);
        assert(!is_empty(id));

        if (id == 1) {
            return array[top1];
        }
        else {
            return array[top2];
        }
    }

    void display() {
        for (int i = top1; i >= 0; --i)
            cout << array[i] << ' ';
        cout << endl;

        for (int i = top2; i < size; ++i)
            cout << array[i] << ' ';
        cout << endl;
    }
};

string reverse_words(string line) {
    int n = line.size();
    Stack<char> stack(n);
    string reversed = "";

    for (int i = 0; i < n; ++i) {
        if (line[i] == ' ') {
            while (!stack.is_empty())
                reversed += stack.top(), stack.pop();
            reversed += ' ';
        }
        else stack.push(line[i]);
    }
    while (!stack.is_empty()) // last word
        reversed += stack.top(), stack.pop();

    return reversed;
}

int reverse_num(int num) {
    if (num == 0) return 0;

    int rev = 0;
    int n = (int)log10(num) + 1;
    Stack<int> stk(n);
    while (num > 0)
        stk.push(num % 10), num /= 10;

    long long expo = 1;
    while(!stk.is_empty())
        rev = stk.pop() * expo + rev, expo *= 10;

    return rev;
}

bool is_open(char c) { return (c == '(' or c == '{' or c == '['); }
bool is_correspond(char close, char open) {
    if (close == ')') return open == '(';
    else if (close == '}') return open == '{';
    else return open == '[';
}
bool is_valid(string str) {
    int n = str.size();
    Stack<char> stk(n);
    bool ret = true;
     for (int i = 0; i < n; ++i) {
         if (is_open(str[i])) stk.push(str[i]);
         else {
             if (is_correspond(str[i], stk.top()))
                 stk.pop();
             else {
                 ret = false;
                 break;
             }
         }
     }

     ret &= stk.is_empty();
     return ret;
}

string remove_duplicates(string str) {
    int n = str.size();
    Stack<char> chars(n);

    for (int i = 0; i < n; ++i) {
        if (chars.is_empty() or str[i] != chars.top()) chars.push(str[i]);
        else chars.pop();
    }

    string result;
    while (!chars.is_empty()) {
        result = chars.pop() + result;
    }

    return result;
}

/////////////////// test //////////////

void test1() {
    stackOf2 stk2(10);

    stk2.push(2, 5);
    stk2.push(2, 6);
    stk2.pop(2);
    stk2.push(2, 7);
    stk2.push(2, 9);

    stk2.push(1, 4);
    cout << stk2.top(1) << endl;
    cout << stk2.top(2) << endl;
    stk2.push(1, 6);
    stk2.push(1, 8);
    stk2.push(2, 3);

    stk2.display();
}

int main()
{
    test1();

    cout << "\nno RTE \n";

    return 0;
}