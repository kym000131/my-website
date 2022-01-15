#include "Tree.h"

template <class keyT, class valT>
class AVLTree_t : public Tree_t<keyT, valT> {
    long check_bf(Node_t<keyT, valT>* n) {
        // Checks balance factor values in tree.
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = check_bf(n->right);
        hl = check_bf(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        long bf = hl - hr;
        assert(bf == n->meta);
        assert(bf < 2);
        assert(bf > -2);
        return h;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    Node_t<keyT, valT>* balance(Node_t<keyT, valT>* a) {
        // Make tree balanced
        short A_meta = a->meta;
        short B_meta = 0;
        if (A_meta > 0) {
            B_meta = a->left->meta;
            if (B_meta >= 0){//not sure about condition
                //LL rotation
                Tree_t<keyT, valT>::rotate(a, true);
            }
            else {
                //LR rotation
                //first rotation
                Node_t<keyT, valT>* first = a->left;
                Tree_t<keyT, valT>::rotate(first, false);
                first->meta = (short)(check_bf(first->left) - check_bf(first->right));
                first->parent->meta = (short)(check_bf(first->parent->left) - check_bf(first->parent->right));
                //second rotation
                Tree_t<keyT, valT>::rotate(a, true);
            }
        }
        else {
            B_meta = a->right->meta;
            if (B_meta <= 0) {
                //RR rotation
                Tree_t<keyT, valT>::rotate(a, false);
            }
            else {
                //RL rotation
                //first rotation
                Node_t<keyT, valT>* first = a->right;
                Tree_t<keyT, valT>::rotate(first, true);
                first->meta = (short)(check_bf(first->left) - check_bf(first->right));
                first->parent->meta = (short)(check_bf(first->parent->left) - check_bf(first->parent->right));
                //second rotation
                Tree_t<keyT, valT>::rotate(a, false);
            }
        }
        a->meta = (short)(check_bf(a->left) - check_bf(a->right));
        a->parent->meta = (short)(check_bf(a->parent->left) - check_bf(a->parent->right));
        return a;
    }

    /*Node_t<keyT, valT>* insert_internal(keyT key, valT value) {
        // Helper function for insert()
    }*/

    /*void remove_node(Node_t<keyT, valT>* n) {
        // Helper function for remove()
    }*/

public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.

        Node_t<keyT, valT>*  New = Tree_t<keyT, valT>::insert_internal(key, value, 0);
        while (New != NULL) {
            short bf = (short)(check_bf(New->left) - check_bf(New->right));
            if (bf <= 1 && bf >= -1) {
                New->meta = bf;
            }
            else {
                balance(New);
            }
            New = New->parent;
        }
        check_bf(Tree_t<keyT, valT>::root);
        Tree_t<keyT, valT>::check_order();
    }

    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.

        //check the tree is not NULL
        if (Tree_t<keyT, valT>::root == NULL) {
            return false;
        }
        
        //find the location of deleting key
        Node_t<keyT, valT>* des = Tree_t<keyT, valT>::search_subtree(Tree_t<keyT, valT>::root, key);
        bool ex = false;
        if (des->left != NULL && des->right != NULL) {
            Node_t<keyT, valT>* r = Tree_t<keyT, valT>::get_min(des->right);
            des = r->parent;
            ex = Tree_t<keyT, valT>::remove(key);
        }
        else if (des->left) {
            des = des->left;
            ex = Tree_t<keyT, valT>::remove(key);
        }
        else if (des->right) {
            des = des->right;
            ex = Tree_t<keyT, valT>::remove(key);
            
        }
        else {
            des = des->parent;
            ex = Tree_t<keyT, valT>::remove(key);
        }


        if (ex) {
            while (des != NULL) {
                short bf = (short)(check_bf(des->left) - check_bf(des->right));
                if (bf <= 1 && bf >= -1) {
                    des->meta = bf;
                }
                else {
                    balance(des);
                }
                des = des->parent;
            }
            check_bf(Tree_t<keyT, valT>::root);
            Tree_t<keyT, valT>::check_order();
            return true;
        }
        else {
            check_bf(Tree_t<keyT, valT>::root);
            Tree_t<keyT, valT>::check_order();
            return false;
        }
    }
};
