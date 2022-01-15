#ifndef STACK_H
#define STACK_H

#include <iostream>

using namespace std;

template <typename type>
struct node {
	type data;
	node* link = NULL;
};

template <typename type>
class Stack {
public:
	node<type>* first;
	node<type>* ptrtop;
	int len;

	// Constructor
	explicit Stack();

	// Destructor
	~Stack();

	// Store new element
	void push(const type& item);

	// Return first element
	type& top() const;

	// Remove first element
	void pop();

	void Print();

	// Return true if empty, false otherwise
	bool empty() const;

	// Number of elements stored
	int size() const;
};

// Implement functions below

template <typename type>
Stack<type>::Stack() {
	ptrtop = NULL;
	first = NULL;
	len = 0;
}

template <typename type>
Stack<type>::~Stack() {
	while (first != NULL) {
		this->pop();
	}
}

template <typename type>
void Stack<type>::push(const type& item) {
	node<type>* NewNode = new node<type>;
	NewNode->data = item;
	if (ptrtop == NULL) {
		ptrtop = NewNode;
		first = NewNode;
		len++;
	}
	else {
		ptrtop->link = NewNode;
		ptrtop = NewNode;
		len++;
	}
}

template <typename type>
type& Stack<type>::top() const {
	return ptrtop->data;
}

template <typename type>
void Stack<type>::pop() {
	node<type>* cur = first;
	if (len == 1) {
		delete cur;
		ptrtop = NULL;
		first = NULL;
	}
	else {
		for (int i = 0; i < len - 2; i++) {
			cur = cur->link;
		}
		delete ptrtop;
		ptrtop = cur;
	}
	len--;
}

template <typename type>
bool Stack<type>::empty() const {
	if (len == 0) {
		return true;
	}
	else return false;
}

template <typename type>
int Stack<type>::size() const {
	return len;
}

template <typename type>
void Stack<type>::Print() {
	node<type>* ptr = first;
	if (len == 0) {
		cout << "" << endl;
	}
	else {
		for (int i = 0; i < len; i++) {
			cout << ptr->data << " ";
			ptr = ptr->link;
		}
		cout << endl;
	}
}

#endif //STACK_H

