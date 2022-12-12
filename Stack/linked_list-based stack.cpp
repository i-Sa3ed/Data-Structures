/// LinkedList-based stack

// @ 4 / 12 / 2022
// @ author: Saeed Ahmed

#include <iostream>
#include <assert.h>
#include <math.h>
#include <map>
#include <algorithm>

using namespace std;

template <typename type>
class Stack {
    // notice that we can make the struct internally
    // this provides more encapsulation
    struct Node {
        type data;
        Node* next {};

        Node(type _data) : data(_data) {}
    };

    Node* head;

public:
    Stack() { head = nullptr; }

    bool isEmpty() { return !head; }

    void push(type value) { // similar to "insert front"
        Node* item = new Node(value);

        if(!item) return; // industry check (in case of full RAM)

        item->next = head;
        head = item;
    }
    type pop() {
        assert(!isEmpty());
        Node* delMe = head;
        type element = head->data;
        head = head->next;
        delete delMe;
        return element;
    }

    type top() {
        assert(!isEmpty());
        return head->data;
    }
};

////////// Applications //////////

string infix_to_postfix(string infix) { // O(n) time
    // for simplicity:
    // input: single digits (0-9), upper and lower letters
    // operators are: + - * / ^ ()
    // no unary operators

    // NOTICE: no evaluation for the letters

    Stack<char> operators;
    string postfix;

    map<char, int> rank;
    rank['+'] = rank['-'] = 1;
    rank['*'] = rank['/'] = 2;
    rank['^'] = 3;
    rank['('] = 0;

    // coding tricks:
    infix += '+'; // lowest priority: force the stack to go empty
    operators.push('#'); // with rank = 0, to avoid isEmpty using

    int n = infix.size();
    for (int i = 0; i < n; ++i) {
        if (isdigit(infix[i]) or isalpha(infix[i]))
            postfix += infix[i];
        else {
            if (infix[i] == '(') operators.push(infix[i]);
            else if (infix[i] == ')') {
                while (operators.top() != '(')
                    postfix += operators.pop();
                operators.pop();
            }
            else {
                while (rank[infix[i]] < rank[operators.top()] or
                        (rank[infix[i]] == rank[operators.top()] and infix[i] != '^')) // handle ^, b. it's right to left
                    postfix += operators.pop();
                operators.push(infix[i]);
            }
        }
    }

    return postfix;
}

double operate(double x, double y, char op) { /// utility function
    switch (op) {
        case '+':
            return x + y;
        case '-':
            return x - y;
        case '*':
            return x * y;
        case '/':
            return x / y;
        case '^':
            return pow(x, y);
        default:
            return 0;
    }
}
double evaluate_postfix(string exp) {
    // expression must have NO spaces

    Stack <double> stk;

    int n = exp.size();
    for(int i = 0; i < n; ++i) {
        if(isdigit(exp[i])) stk.push(int(exp[i] - '0'));
        else {
            double y = stk.pop(), x = stk.pop(); // remove last 2 elements
            stk.push(operate(x, y, exp[i]));
        }
    }

    return stk.top();
}

string infixToPrefix(string infix) {
    // Differences:
        /// reverse the input (paras)
        // insert front to "prefix"
        // the rank equality case

    Stack<char> operators;
    string prefix;

    map<char, int> rank;
    rank['+'] = rank['-'] = 1;
    rank['*'] = rank['/'] = 2;
    rank['^'] = 3;

    int n = infix.size();
    string reversedInfix;
    for(int i = n - 1; i >= 0; --i) {
        if (infix[i] == '(')
            reversedInfix += ')';
        else if (infix[i] == ')')
            reversedInfix += '(';
        else reversedInfix += infix[i];
    }
    infix = reversedInfix;

    for (int i = 0; i < n; ++i) {
        if (isdigit(infix[i]) or isalpha(infix[i]))
            prefix = infix[i] + prefix;
        else {
            if (infix[i] == '(') operators.push(infix[i]);
            else if (infix[i] == ')') {
                while (operators.top() != '(')
                    prefix = operators.pop() + prefix;
                operators.pop();
            }
            else {
                while (!operators.isEmpty()) {
                    if (rank[infix[i]] < rank[operators.top()] or
                        (rank[infix[i]] == rank[operators.top()] and infix[i] == '^'))
                        prefix = operators.pop() + prefix;
                    else break;
                }
                operators.push(infix[i]);
            }
        }
    }
    while (!operators.isEmpty())
        prefix = operators.pop() + prefix;

    return prefix;
}

char calc(int sing, char oper) { /// utility function
    if (sing == 1) return oper;
    else return oper == '+'? '-' : '+'; // flip
}
string remove_brackets(string input) {
    // input: numbers, +, -, ()

    Stack<int> signs;
    signs.push(1);

    input = '+' + input; // boundary
    int n = input.size();
    string output;
    for (int i = 1; i < n; ++i) {
        if (input[i] == '(') {
            if (input[i - 1] == '-') signs.push(-1*signs.top()); // reverse it
            else signs.push(signs.top()); // keep as it is
        }
        else if (input[i] == ')')
            signs.pop();
        else {
            if (isdigit(input[i]))
                output += input[i];
            else
                output += calc(signs.top(), input[i]);
        }
    }

    return output;
}


////////// test & main //////////

void test() {
    string test;

    test = "1+2-3-4+5-6-7+8";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "9-(2+3)";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "9-(2-3)";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "9+(2-3)";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(2-3-(4+5))-6-(7-8)";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(2-3-(4+5)+6-7)";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(2-3-(4+5-(6-7)))";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-((4+5)-(6-7)))";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(((4-5)-(6-7))))";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(2-3-((4+5)-(6-7)))";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(2-3-((4-5)-(6-7)))";
    cout << test << " => " << remove_brackets(test) << "\n";

    test = "1-(2-3+((4-5)-(6-7)))";
    cout << test << " => " << remove_brackets(test) << "\n";
}

int main() {
    test();

    return 0;
}