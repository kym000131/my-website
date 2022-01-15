#include <iostream>
#include <cstdlib>

#include "AVLTree.h"

int main() {
    AVLTree_t<int, int> t;

    // basic insertions
    t.insert(3, 5);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(-2, 7);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(10, 4);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(7, 8);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';

    // basic search
    t.search(7);
    t.search(-2);

    // basic removal
    t.remove(3);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';

    // update on existing keys
    t.insert(-2, 8);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(10, 11);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';

    // insertion of "special" keys and values
    t.insert(-1, 3);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(4, -1);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(0, 1);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.insert(1, 0);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';

    // searching those
    t.search(-1);
    t.search(0);

    // and removals
    t.remove(-1);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.remove(0);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';

    // try to search for nonexisting keys
    t.search(1000);
    t.search(-42);

    // and removals
    t.remove(100);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';
    t.remove(-1);
    std::cout << t.to_string_pre_order() << '\n';
    std::cout << t.to_string_level_order() << '\n';

    srand(15);

    //random insertion
    AVLTree_t<int, int> avl1;
    for (int i = 0; i < 100; i++) {
        int k = rand();
        avl1.insert(k, 5);
    }

    //random delete
    AVLTree_t<int, int> avl2;
    for (int i = 0; i < 1000; i++) {
        avl2.insert(i, 5);
    }

    cout << endl;
    for (int i = 0; i < 500; i++) {
        int j = rand() % 1000;
        cout << j << endl;
        avl2.remove(j);
    }

    cout << endl;
    cout << avl2.to_string_level_order() << endl;



    // These tests are by no means exhaustive
    // Try to think of your own edge cases!
    // (Something more closely related to tree structure itself would be a good start)

    return 0;
}
