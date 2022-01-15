#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

using namespace std;

template <typename type>
struct Qnode {
	type data;
	Qnode* link=NULL;
};

template <typename type>
class Queue {

public:
	Qnode<type>* frontptr;
	Qnode<type>* rearptr;
	int len;

	// Constructor
	explicit Queue();

	// Destructor
	~Queue();

	// Store new element
	void push(const type& item);

	// Return first element
	type& front() const;

	// Remove first element
	void pop();

	// Return true if empty, false otherwise
	bool empty() const;

	// Number of elements stored
	int size() const;

};

// Implement functions below

template <typename type>
Queue<type>::Queue() {
	frontptr = NULL;
	rearptr = NULL;
	len = 0;
}

template <typename type>
Queue<type>::~Queue() {
	while (frontptr != NULL) {
		this->pop();
	}
}


template <typename type>
void Queue<type>::push(const type& item) {
	Qnode<type>* NewNode = new Qnode<type>;
	NewNode->data = item;
	if (rearptr == NULL) {
		rearptr = NewNode;
		frontptr = NewNode;
		len++;
	}
	else {
		rearptr->link = NewNode;
		rearptr = NewNode;
		len++;
	}
}

template <typename type>
type& Queue<type>::front() const {
	return frontptr->data;
}

template <typename type>
void Queue<type>::pop() {
	Qnode<type>* cur = frontptr;
	if (len == 1) {
		delete cur;
		frontptr = NULL;
		rearptr = NULL;
		len--;
	}
	else {
		cur = cur->link;
		delete frontptr;
		frontptr = cur;
		len--;
	}
}

template <typename type>
bool Queue<type>::empty() const {
	if (len == 0) {
		return true;
	}
	else return false;
}

template <typename type>
int Queue<type>::size() const {
	return len;
}

#endif //QUEUE_H

