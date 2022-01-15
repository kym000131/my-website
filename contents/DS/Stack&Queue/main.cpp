#include <iostream>
//#include <crtdbg.h>
#include "stack.h"
#include "queue.h"
#include "tree.h"
#include "to_postfix.h"

int main() {
    // part 1: stack & queue
    Stack<int> s;
    for (int i = 0; i < 8; ++i) {
        s.push(i);
    }
    while (!s.empty()) {
        cout << s.top() << " "; // 7 6 5 4 3 2 1 0
        s.pop();
    }
    cout << endl;

    Queue<int> q;
    for (int i = 0; i < 8; ++i) {
        q.push(i);
    }
    while (!q.empty()) {
        cout << q.front() << " "; // 0 1 2 3 4 5 6 7
        q.pop();
    }
    cout << endl;

    // part 2: convert infix to postfix
    string infix = "-1 * (2 + -3 * 4) / -2";
    string infix1 = "(7+4)*2-1";
    string infix2 = "-10 + 3 - 2";
    string infix3 = " -(2 + -3 * 4) / -2";
    string infix4 = "((2+3*5)+7)";
    string infix5 = "-10-((-2+(2+4*3))-12)+122*(123+(120+888)-300)";
    string infix6 = "((-2))";
    string postfix = to_postfix(infix);
    string postfix1 = to_postfix(infix1);
    string postfix2 = to_postfix(infix2);
    string postfix3 = to_postfix(infix3);
    string postfix4 = to_postfix(infix4);
    string postfix5 = to_postfix(infix5);
    string postfix6 = to_postfix(infix6);
    cout << postfix << "end" << endl; // -1 2 -3 4 * + * -2 /
    cout << postfix1 << "end" << endl;
    cout << postfix2 << "end" << endl;
    cout << postfix3 << "end" << endl;
    cout << postfix4 << "end" << endl;
    cout << postfix5 << "end" << endl;
    cout << postfix6 << "end" << endl;


    // part 3: evaluate an expression tree
    Tree* postfix_tree = build_expression_tree(postfix);
    Tree* postfix_tree1 = build_expression_tree(postfix1);
    Tree* postfix_tree2 = build_expression_tree(postfix2);
    Tree* postfix_tree3 = build_expression_tree(postfix3);
    Tree* postfix_tree4 = build_expression_tree(postfix4);
    Tree* postfix_tree5 = build_expression_tree(postfix5);
    Tree* postfix_tree6 = build_expression_tree(postfix6);
    postfix_tree->print();// (/,*,-2,-1,+,null,null,null,null,2,*,null,null,-3,4)
    cout << endl;
    postfix_tree1->print();
    cout << endl;
    postfix_tree2->print();
    cout << endl;
    postfix_tree3->print();
    cout << endl;
    postfix_tree4->print();
    cout << endl;
    postfix_tree5->print();
    cout << endl;
    postfix_tree6->print();
    cout << endl;
    cout << postfix_tree->eval() << endl;// -5
    cout << postfix_tree1->eval() << endl;
    cout << postfix_tree2->eval() << endl;
    cout << postfix_tree3->eval() << endl;
    cout << postfix_tree4->eval() << endl;
    cout << postfix_tree5->eval() << endl;
    cout << postfix_tree6->eval() << endl;

    delete postfix_tree;
    delete postfix_tree1;
    delete postfix_tree2;
    delete postfix_tree3;
    delete postfix_tree4;
    delete postfix_tree5;
    delete postfix_tree6;

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return 0;
}

