#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "stack.h"
#include "queue.h"

using namespace std;

/*
 * Recursive binary tree structure for building expression tree
 *
 * Constructors: Tree(char, Tree*, Tree*) and Tree(string, Tree*, Tree*)
 *      pointers will be automatically set to NULL if omitted
 *
 * Destructor: recursively destruct whole tree
 *
 */

struct Tree {
    string expr;
    Tree* left;
    Tree* right;

    explicit Tree(char h, Tree* l = NULL, Tree* r = NULL) {
        expr = string(1, h);
        left = l;
        right = r;
    }

    explicit Tree(const string& h = string(), Tree* l = NULL, Tree* r = NULL) {
        expr = h;
        left = l;
        right = r;
    }

    ~Tree() {
        delete left;
        delete right;
    }

    int size() {
        int size = 1;
        if (left != NULL) { size += left->size(); }
        if (right != NULL) { size += right->size(); }
        return size;
    }

    void print();

    double eval();
};

// -------- implement below --------

void Tree::print() {
    Queue<Tree*> q;
    Tree* cur = this;
    int size = this->size();
    int i = 0;
    cout << "(";
    while (i<size) {
        cout << cur->expr;
        if (cur->expr == "null") {
            if (q.empty()) return;
            cur = q.front();
            q.pop();
            i--;
        }
        else {
            if (cur->left) {
                q.push(cur->left);
            }
            else {
                Tree* node = new Tree;
                node->expr = "null";
                q.push(node);
            }

            if (cur->right) {
                q.push(cur->right);
            }
            else {
                Tree* node = new Tree;
                node->expr = "null";
                q.push(node);
            }

            if (q.empty()) return;

            cur = q.front();
            q.pop();
        }
        if (i != size - 1) {
            cout<<",";
        }
        i++;
    }
    cout << ")";
}

double Tree::eval() {
    Tree* cur = this;
    if (cur) {
        double l_val=cur->left->eval();
        double r_val=cur->right->eval();
        if (cur->expr == "+") {
            return l_val + r_val;
        }
        else if (cur->expr == "*") {
            return l_val * r_val;
        }
        else if (cur->expr == "/") {
            return l_val / r_val;
        }
        else if (cur->expr == "-") {
            return l_val - r_val;
        }
        else {
            stringstream ss;
            ss.str(cur->expr);
            double d;
            ss >> d;
            return d;
        }
    }
}

Tree* build_expression_tree(const string& postfix) {
    stringstream ss;
    ss.str(postfix);
    string pf;
    Stack<Tree*> trs;
    Tree* root = NULL;
    int test = 0;
    while (ss >> pf) {
        if (pf == "+" || pf == "-" || pf == "/" || pf == "*") {
            Tree* node = new Tree;
            node->expr = pf;
            Tree* n1 = trs.top();
            trs.pop();
            Tree* n2 = trs.top();
            trs.pop();
            node->left = n2;
            node->right = n1;
            root = node;
            trs.push(node);
        }
        else {
            Tree* node = new Tree;
            node->expr = pf;
            root = node;
            trs.push(node);
        }
        test++;
    }
    return root;
}

#endif //TREE_H


