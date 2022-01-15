#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Tree.h"
#include "AVLTree.h"
#include "RBTree.h"
using namespace std;

int main() {

	srand(77);

	AVLTree_t<int, int> avltree;
	for (int i = 0; i < 1000; i++) {
		avltree.insert(i, i + 2);
	}

	for (int i = 1000; i > 0; i--) {
		avltree.remove(i);
	}

	for (int i = 1000; i >= 0; i--) {
		avltree.insert(i, i + 2);
	}

	cout << avltree.remove(0) << endl;
	cout << avltree.remove(0) << endl;

	for (int j = 0; j < 500; j++) {
		int k = rand() % 1000;
		avltree.remove(k);
		cout << " avl remove number: " << k << ",";
		cout << "avl remove result: " << avltree.remove(k) << endl;
	}

	cout << avltree.to_string_level_order() << endl;

	cout << "---------------------------------------------------------------------------------------------" << endl;


	RBTree_t<int, int> rbtree;
	
	for (int i = 0; i < 100; i++) {
		rbtree.insert(i, 5);
	}

	for (int j = 0; j < 90; j++) {
		int k = rand() % 100;
		cout << "rb remove number: " << k << ",";
		cout << "rb remove result: " << rbtree.remove(k) << endl;
	}

	cout << rbtree.to_string_level_order() << endl;

	cout << "--------------------------------------------------------------------------------------------------" << endl;

	/*AVLTree_t<int, int> at;

	for (int i = 0; i < 100; i++) {
		at.insert(i, 5);
	}

	for (int i = 0; i < 91; i++) {
		at.remove(i);
	}

	cout << at.to_string_level_order() << endl;*/
 
	return 0;
}