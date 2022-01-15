// CSE221 Assignment 1

#ifndef LinkedList_h
#define LinkedList_h

#include <typeinfo>
#include <iostream>
using namespace std;

template <typename T>
struct node {
	T data;
	node* next = NULL;
};

template <typename Type>
class LinkedList
{
private:
	node<Type>* first = NULL;
	int len = 0;

public:
	// Constructor
	LinkedList();

	// Destructor
	~LinkedList();

	// Get the value located at index
	Type Get(const int index);

	// Add val at head
	void AddAtHead(const Type& val);

	// Add val at index
	void AddAtIndex(const int index, const Type& val);

	// Delete an element located at index
	void DeleteAtIndex(const int index);

	// Delete val in linked list
	void DeleteValue(const Type& val);

	// Move the first element of val to head
	void MoveToHead(const Type& val);

	// Rotate the linked list right by steps times
	void Rotate(const int steps);

	// Reduce value that repeats multiple times
	void Reduce();

	// Reverse at every k number of nodes at a time
	void K_Reverse(const int k);

	// Sort even and odd numbers separately then append
	void EvenOddSeparateSort();

	// Return the number of elements in the linked list
	int Size();

	// Delete all elements from the linked list
	void CleanUp();
	
	// Print the list
	void Print();

};

/*
 *	Implementation
 */

template <typename Type>
LinkedList<Type>::LinkedList(){}

template <typename Type>
LinkedList<Type>::~LinkedList()
{
	CleanUp();
}

template <typename Type>
Type LinkedList<Type>::Get(const int index)
{
	if (index < 0 || index >= len) {
		cout << "invaild index" << endl;
		return -1;
	}
	else {
		node<Type>* cursor = first;
		for (int i = 0; i < index; i++) {
			cursor = cursor->next;
		}
		return cursor->data;
	}
}

template <typename Type>
void LinkedList<Type>::AddAtHead(const Type& val)
{
	node<Type> *NewNode = new node<Type>; 
	NewNode->data = val;
	if (len == 0) {
		first = NewNode;
		NewNode->next = NULL;
		len++;
	}
	else {
		node<Type>* cursor = first;
		first = NewNode;
		NewNode->next = cursor;
		len++;
	}
}

template <typename Type>
void LinkedList<Type>::AddAtIndex(const int index, const Type& val)
{
	if (index < 0 || index > len) {
		cout << "out of index" << endl;
		return;
	}
	else if (index == 0) {
		AddAtHead(val);
	}
	else {
		node<Type>* NewNode = new node<Type>;
		NewNode->data = val;
		node<Type>* cursor = first;
		for (int i = 0; i < index-1; i++) {
			cursor = cursor->next;
		}
		if (index == len) {
			NewNode->next = NULL;
		}
		else {
			NewNode->next = cursor->next;
		}
		cursor->next = NewNode;
		len++;
	}
}

template <typename Type>
void LinkedList<Type>::DeleteAtIndex(const int index)
{
	if (index < 0 || index >= len) {
		cout << "out of index" << endl;
		return;
	}
	else {
		node<Type>* des = first;
		for (int i = 0; i < index; i++) {
			des = des->next;
		}
		if (index == len - 1) {
			delete des;
			len--;
		}
		else if (index == 0) {
			first = first->next;
			delete des;
			len--;
		}
		else {
			node<Type>* predes = first;
			for (int j = 0; j < index - 1; j++) {
				predes = predes->next;
			}
			predes->next = des->next;
			delete des;
			len--;
		}
	}
}

template <typename Type>
void LinkedList<Type>::DeleteValue(const Type& val)
{
	int test = 0;
	node<Type>* des = first;
	for (int i = 0; i < len; i++) {
		if (des->data == val) {
			if (i == len - 1) {
				delete des;
				len--;
				test++;
			}
			else if (i == 0) {
				first = first->next;
				delete des;
				len--;
				test++;
			}
			else {
				node<Type>* predes = first;
				for (int j = 0; j < i - 1; j++) {
					predes = predes->next;
				}
				predes->next = des->next;
				delete des;
				len--;
				test++;
			}
			return;
		}
		des = des->next;
	}

	if (test == 0) {
		cout << "there is no such value" << endl;
	}
}
	/*node<Type>* predes = first;
	node<Type>* des = predes->next;
	for (int i = 0; i <= len - 1; i++) {
		if (des->data == val) {
			predes->next = des->next;
			delete des;
			len--;
		}
		else {
			predes = predes->next;
			des = des->next;
		}
	}뭐가 달라?*/


template <typename Type>
void LinkedList<Type>::MoveToHead(const Type& val)
{
	int init_len = len;
	DeleteValue(val);
	if (init_len != len) {
		AddAtHead(val);
	}
}

template <typename Type>
void LinkedList<Type>::Rotate(const int steps)
{
	if (steps >= 0) {
		if (steps > 0) {
			for (int i = 0; i < steps; i++) {
				Type last_data;
				node<Type>* last = first;
				for (int j = 0; j < len - 1; j++) {
					last = last->next;
				}
				last_data = last->data;
				MoveToHead(last_data);
			}
		}
	}
	//end를 만들어서 그쪽부터 접근하는거랑 뭐가 더 빠를까?
	/*이 함수 내에서만 end를 만들기에는
	MovetoHead에서 이미 delete를 해버림*/
}

template <typename Type>
void LinkedList<Type>::Reduce()
{
	for (int i = 0; i < len; i++) {
		Type value = Get(i);
		for (int j = i+1; j < len; j++) {
			if(j>i){
				if (Get(j) == value) {
					DeleteAtIndex(j);
					j--;
				}
			}
		}
	}
}

template <typename Type>
void LinkedList<Type>::K_Reverse(const int k)
{
	if (k > 0 && k <= len) {
		int init_len = len;
		int iter = len / k;
		LinkedList<Type> k_re;
		int kr_index = 0;
		for (int i = 1; i <= iter; i++) {
			for (int j = 1; j <= k; j++) {
				k_re.AddAtIndex(kr_index, Get(k * i - j));
				kr_index++;
			}
		}
		int remain = k * iter;
		for (int r = remain; r < init_len; r++) {
			k_re.AddAtIndex(r, Get(r));
		}
		this->CleanUp();
		for (int t = 0; t < init_len; t++) {
			this->AddAtIndex(t, k_re.Get(t));
		}
	}
}

template <typename Type>
void LinkedList<Type>::EvenOddSeparateSort() 
{
	LinkedList<Type> even;
	LinkedList<Type> odd;
	for (int i = 0; i < len; i++) {
		if (Get(i) % 2 == 0) {
			even.AddAtHead(Get(i));
		}
		else {
			odd.AddAtHead(Get(i));
		}
	}
	for (int i = 0; i < odd.Size(); i++) {
		Type o_num = odd.Get(i);
		for (int j = i + 1; j < odd.Size(); j++) {
			Type compare = odd.Get(j);
			if (compare < o_num) {
				o_num = compare;
			}
		}
		odd.DeleteValue(o_num);
		odd.AddAtHead(o_num);
		int init_odd = odd.Size();
		odd.Reduce();
		if (odd.Size() != init_odd) {
			odd.AddAtHead(o_num);
		}
	}
	for (int i = 0; i < even.Size(); i++) {
		Type e_num = even.Get(i);
		for (int j = i + 1; j < even.Size(); j++) {
			Type compare = even.Get(j);
			if (compare < e_num) {
				e_num = compare;
			}
		}
		even.DeleteValue(e_num);
		even.AddAtHead(e_num);
		int init_even = even.Size();
		even.Reduce();
		if (even.Size() != init_even) {
			even.AddAtHead(e_num);
		}
	}
	this->CleanUp();
	for (int o_index = odd.Size(); o_index > 0; o_index--) {
		this->AddAtHead(odd.Get(o_index-1));
	}
	for (int e_index = 0; e_index < even.Size(); e_index++) {
		this->AddAtHead(even.Get(e_index));
	}
}

template <typename Type>
int LinkedList<Type>::Size()
{
	return len;
}

template <typename Type>
void LinkedList<Type>::CleanUp()
{
	while (len != 0) {
		DeleteAtIndex(0);
	}
}

template <typename Type>
void LinkedList<Type>::Print()
{
	cout << "(";
	for (int i = 0; i < len; i++) {
		cout << Get(i);
		if (i != len - 1) {
			cout << ",";
		}
	}
	cout << ")";
	cout << endl;
}


#endif