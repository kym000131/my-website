#include "Tree.h"

#define BLACK 0
#define RED 1

template <class keyT, class valT>
class RBTree_t : public Tree_t<keyT, valT> {
    size_t check_rb(Node_t<keyT, valT>* n) {
        // Checks RB tree property
        size_t b, br, bl;
        br = bl = b = 1;
        assert(n);
        if(n && n->right) {
            br = check_rb(n->right);
            assert(n->meta == BLACK || n->right->meta == BLACK);
        }
        if(n && n->left) {
            bl = check_rb(n->left);
            assert(n->meta == BLACK || n->left->meta == BLACK);
        }
        b = br;
        if(n == this->root && br != bl) {
            //dump_subtree(this->root);
        }
        assert(n != this->root || br == bl);
        b = br;
        if(n->meta == BLACK) b += 1;
        if(n == this->root) assert(n->meta == BLACK);

        return b;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    void balance(Node_t<keyT, valT>* n) {
        // Make tree balanced

        //components to judging situation
        int R_n_pp = 0; // 0 is left, 1 is right
        int R_pp_gp = 0;
        Node_t<keyT, valT>* pp = n->parent;
        if (n == pp->left) {
            R_n_pp = 0;
        }
        else {
            R_n_pp = 1;
        }
        Node_t<keyT, valT>* gp = pp->parent;
        if (pp == gp->left) {
            R_pp_gp = 0;
        }
        else {
            R_pp_gp = 1;
        }
        Node_t<keyT, valT>* uncle = NULL;
        if (R_pp_gp == 0) {
            uncle = gp->right;
        }
        else {
            uncle = gp->left;
        }

        //balancing cases
        if (uncle == NULL || uncle->meta == BLACK) {
            if (R_n_pp == 0 && R_pp_gp == 0) {
                //LLb
                Tree_t<keyT, valT>::rotate(gp, true);
                if (pp->meta == RED) pp->meta = BLACK;
                if (gp->meta == BLACK) gp->meta = RED;
            }
            else if (R_n_pp == 1 && R_pp_gp == 0) {
                //LRb
                Tree_t<keyT, valT>::rotate(pp, false);
                Tree_t<keyT, valT>::rotate(gp, true);
                if (n->meta == RED) n->meta = BLACK;
                if (gp->meta == BLACK) gp->meta = RED;
            }
            else if (R_n_pp == 1 && R_pp_gp == 1) {
                //RRb
                Tree_t<keyT, valT>::rotate(gp, false);
                if (pp->meta == RED) pp->meta = BLACK;
                if (gp->meta == BLACK) gp->meta = RED;
            }
            else {
                //RLb
                Tree_t<keyT, valT>::rotate(pp, true);
                Tree_t<keyT, valT>::rotate(gp, false);
                if (n->meta == RED) n->meta = BLACK;
                if (gp->meta == BLACK) gp->meta = RED;
            }
        }
        else {
            //XYr
            flip(gp);
            if (gp->parent) {
                if (gp->parent->meta == RED) {
                    balance(gp);
                }
            }
        }
        Tree_t<keyT, valT>::root->meta = BLACK;
    }

    void flip(Node_t<keyT, valT>* n) {
        // Flip the color of this node and all children

        if (n->meta == BLACK) {
            n->meta = RED;
        }
        else {
            n->meta = BLACK;
        }
        if (n->right->meta == BLACK) {
            n->right->meta = RED;
        }
        else {
            n->right->meta = BLACK;
        }
        if (n->left->meta == BLACK) {
            n->left->meta = RED;
        }
        else {
            n->left->meta = BLACK;
        }
    }

    void remove_node(Node_t<keyT, valT>* n) {
        // Helper function for remove()
        Tree_t<keyT, valT>::remove_node(n);
    }

    bool case_0(Node_t<keyT, valT>* double_b_pp, bool direction) {

        Node_t<keyT, valT>* sibling = NULL;
        if (direction) {
            sibling = double_b_pp->left;
        }
        else {
            sibling = double_b_pp->right;
        }

        if (sibling->meta == BLACK) {
            if (!sibling->left && !sibling->right) {
                sibling->meta = RED;
                if (double_b_pp->meta == BLACK) {
                    if (double_b_pp == Tree_t<keyT, valT>::root) {
                        return true;
                    }

                    if (double_b_pp == double_b_pp->parent->left) {
                        r_balance(double_b_pp->parent, false);
                        return true;
                    }
                    else {
                        r_balance(double_b_pp->parent, true);
                        return true;
                    }
                }
                else return false;
            }
            else if (sibling->left && sibling->right) {
                if (sibling->left->meta == BLACK && sibling->right->meta == BLACK) {
                    sibling->meta = RED;
                    if (double_b_pp->meta == BLACK) {
                        if (double_b_pp == Tree_t<keyT, valT>::root) {
                            return true;
                        }

                        if (double_b_pp == double_b_pp->parent->left) {
                            r_balance(double_b_pp->parent, false);
                            return true;
                        }
                        else {
                            r_balance(double_b_pp->parent, true);
                            return true;
                        }
                    }
                }
                else return false;
            }
            else return false;
        }
        return false;
    }

    void case_1(Node_t<keyT, valT>* double_b_pp, bool direction) {
        Node_t<keyT, valT>* sibling = NULL;
        if (direction) {
            sibling = double_b_pp->left;
        }
        else {
            sibling = double_b_pp->right;
        }

        sibling->meta = RED;

        if (direction) {
            Tree_t<keyT, valT>::rotate(double_b_pp, true);
        }
        else {
            Tree_t<keyT, valT>::rotate(double_b_pp, false);
        }

        short temp = double_b_pp->meta;
        double_b_pp->meta = sibling->meta;
        sibling->meta = temp;

        case_sort(double_b_pp, direction);
        return;
    }

    void case_2(Node_t<keyT, valT>* double_b_pp, bool direction) {
        Node_t<keyT, valT>* sibling = NULL;
        if (direction) {
            sibling = double_b_pp->left;
        }
        else {
            sibling = double_b_pp->right;
        }

        sibling->meta = RED;
        double_b_pp->meta = BLACK;

        return;
    }

    void case_3(Node_t<keyT, valT>* double_b_pp, bool direction) {
        Node_t<keyT, valT>* sibling = NULL;
        if (direction) {
            sibling = double_b_pp->left;
        }
        else {
            sibling = double_b_pp->right;
        }

        if (!direction) {
            sibling->meta = RED;
            sibling->left->meta = BLACK;
            Tree_t<keyT, valT>::rotate(sibling, true);
        }
        else {
            sibling->meta = RED;
            sibling->right->meta = BLACK;
            Tree_t<keyT, valT>::rotate(sibling, false);
        }

        case_4(double_b_pp, direction);
        return;
    }

    void case_4(Node_t<keyT, valT>* double_b_pp, bool direction) {

        Node_t<keyT, valT>* sibling = NULL;
        Node_t<keyT, valT>* red_node = NULL;
        if (direction) {
            sibling = double_b_pp->left;
            red_node = sibling->left;
            Tree_t<keyT, valT>::rotate(double_b_pp, true);
        }
        else {
            sibling = double_b_pp->right;
            red_node = sibling->right;
            Tree_t<keyT, valT>::rotate(double_b_pp, false);
        }

        short temp = double_b_pp->meta;
        double_b_pp->meta = double_b_pp->parent->meta;
        double_b_pp->parent->meta = temp;

        red_node->meta = BLACK;

        return;
    }


    

    void case_sort(Node_t<keyT, valT>* double_b_pp, bool direction) {

        Node_t<keyT, valT>* sibling = NULL;
        if (direction) {
            sibling = double_b_pp->left;
        }
        else {
            sibling = double_b_pp->right;
        }

        if (double_b_pp->meta == BLACK && sibling->meta == RED) {
            case_1(double_b_pp, direction);
            return;
        }

        if (double_b_pp->meta == RED && sibling->meta == BLACK && sibling->left == NULL && sibling->right == NULL) {
            case_2(double_b_pp, direction);
            return;
        }
        else if (double_b_pp->meta == RED && sibling->meta == BLACK && sibling->left == NULL && sibling->right != NULL) {
            if (sibling->right->meta == BLACK) {
                case_2(double_b_pp, direction);
                return;
            }
            else {
                if (direction) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else {
                    case_4(double_b_pp, direction);
                    return;
                }
            }
            
        }
        else if (double_b_pp->meta == RED && sibling->meta == BLACK && sibling->left != NULL && sibling->right == NULL) {
            if (sibling->left->meta == BLACK) {
                case_2(double_b_pp, direction);
                return;
            }
            else {
                if (!direction) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else {
                    case_4(double_b_pp, direction);
                    return;
                }
            }

        }
        else if (double_b_pp->meta == RED && sibling->meta == BLACK && sibling->left != NULL && sibling->right != NULL) {
            if (sibling->left->meta == BLACK && sibling->right->meta == BLACK) {
                case_2(double_b_pp, direction);
                return;
            }
            else if (sibling->left->meta == RED && sibling->right->meta == BLACK) {
                if (!direction) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else {
                    case_4(double_b_pp, direction);
                    return;
                }
            }
            else if (sibling->left->meta == BLACK && sibling->right->meta == RED) {
                if (direction) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else {
                    case_4(double_b_pp, direction);
                    return;
                }
            }
            else {
                case_4(double_b_pp, direction);
                return;
            }
        }
        else {
            if (sibling->meta == BLACK) {
                if (sibling->left == NULL && sibling->right == NULL) {
                    case_sort(double_b_pp, direction);
                    return;
                }
                else if (sibling->left != NULL && sibling->right == NULL) {
                    if (sibling->left->meta == RED) {
                        if (!direction) {
                            case_3(double_b_pp, direction);
                            return;
                        }
                        else {
                            case_4(double_b_pp, direction);
                            return;
                        }
                    }
                    else {
                        case_sort(double_b_pp, direction);
                        return;
                    }
                }
                else if (sibling->left == NULL && sibling->right != NULL) {
                    if (sibling->right->meta == RED) {
                        if (direction) {
                            case_3(double_b_pp, direction);
                            return;
                        }
                        else {
                            case_4(double_b_pp, direction);
                            return;
                        }
                    }
                    else {
                        case_sort(double_b_pp, direction);
                        return;
                    }
                }
                else {
                    if (sibling->left->meta == RED && sibling->right->meta == RED) {
                        case_4(double_b_pp, direction);
                        return;
                    }
                    else if (sibling->left->meta == BLACK && sibling->right->meta == RED) {
                        if (direction) {
                            case_3(double_b_pp, direction);
                            return;
                        }
                        else {
                            case_4(double_b_pp, direction);
                            return;
                        }
                    }
                    else if (sibling->left->meta == RED && sibling->right->meta == BLACK) {
                        if (!direction) {
                            case_3(double_b_pp, direction);
                            return;
                        }
                        else {
                            case_4(double_b_pp, direction);
                            return;
                        }
                    }
                    else {
                        case_sort(double_b_pp, direction);
                        return;
                    }
                }
            }
            else {
                case_sort(double_b_pp, direction);
                return;
            }
        }
        /*else if (double_b_pp->meta == RED && sibling->meta == BLACK && sibling->left->meta == BLACK && sibling->right->meta == BLACK) {
            case_2(double_b_pp, direction);
            return;
        }
        else {
            if (!direction) {
                if (sibling->meta == BLACK && sibling->right == NULL && sibling->left->meta == RED) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else if (sibling->meta == BLACK && sibling->right->meta == BLACK && sibling->left->meta == RED) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else if (sibling->meta == BLACK && sibling->right->meta == RED) {
                    case_4(double_b_pp, direction);
                    return;
                }
            }
            else {
                if (sibling->meta == BLACK && sibling->left == NULL && sibling->right->meta == RED) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else if (sibling->meta == BLACK && sibling->left->meta == BLACK && sibling->right->meta == RED) {
                    case_3(double_b_pp, direction);
                    return;
                }
                else if (sibling->meta == BLACK && sibling->left->meta == RED) {
                    case_4(double_b_pp, direction);
                    return;
                }
            }
        }*/
        return;
    }

    //direction true: right, false: left
    void r_balance(Node_t<keyT, valT>* double_b_pp, bool direction) {

        /*Node_t<keyT, valT>* sibling = NULL;
        if (direction) {
            sibling = double_b_pp->left;
        }
        else {
            sibling = double_b_pp->right;
        }*/
        
        bool c0 = false;
        
        if (double_b_pp->meta == BLACK) {
            c0 = case_0(double_b_pp, direction);
        }

        if (!c0) {
            case_sort(double_b_pp, direction);
        }
        
        return;
    }

public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.

        Node_t<keyT, valT>* i_loc = NULL;
        bool exist = false;
        if (Tree_t<keyT, valT>::root == NULL) {
            i_loc = Tree_t<keyT, valT>::insert_internal(key, value, BLACK);
        }
        else {
            exist = this->search(key).valid;
            i_loc = Tree_t<keyT, valT>::insert_internal(key, value, RED);
        }

        if (!exist) {
            if (i_loc->parent) {
                if (i_loc->parent->meta == RED) {
                    balance(i_loc);
                }
            }
        }
        check_rb(Tree_t<keyT, valT>::root);
        Tree_t<keyT, valT>::check_order();
    }


    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.

        //check the tree is not NULL
        if (Tree_t<keyT, valT>::root == NULL) {
            return false;
        }

        //set the successor
        Node_t<keyT, valT>* des = Tree_t<keyT, valT>::search_subtree(Tree_t<keyT, valT>::root, key);
        if (des->key == key) {

            if (des == Tree_t<keyT, valT>::root) {
                if (Tree_t<keyT, valT>::root->left == NULL && Tree_t<keyT, valT>::root->right == NULL) {
                    remove_node(des);
                    return true;
                }
            }

            Node_t<keyT, valT>* succ = NULL;

            if (des->left != NULL && des->right != NULL) {
                succ = Tree_t<keyT, valT>::get_min(des->right);
                des->key = succ->key;
                des->value = succ->value;
            }
            else {
                succ = des;
            }

            //define situation
            if (succ->meta == RED) {
                remove_node(succ);
            }
            else {
                if (succ->left) {
                    if (succ->left->meta == RED) {
                        succ->left->meta = BLACK;
                        remove_node(succ);
                    }
                    else {
                        Node_t<keyT, valT>* d_pp = succ->parent;
                        remove_node(succ);
                        r_balance(d_pp, false);
                    }
                }
                else if (succ->right) {
                    if (succ->right->meta == RED) {
                        succ->right->meta = BLACK;
                        remove_node(succ);
                    }
                    else {
                        Node_t<keyT, valT>* d_pp = succ->parent;
                        remove_node(succ);
                        r_balance(d_pp, true);
                    }
                }
                else {
                    Node_t<keyT, valT>* d_pp = succ->parent;
                    if (succ == d_pp->left) {
                        remove_node(succ);
                        r_balance(d_pp, false);
                    }
                    else {
                        remove_node(succ);
                        r_balance(d_pp, true);
                    }
                }
            } 
            check_rb(Tree_t<keyT, valT>::root);
            Tree_t<keyT, valT>::check_order();
            return true;
        }
        else {
            check_rb(Tree_t<keyT, valT>::root);
            Tree_t<keyT, valT>::check_order();
            return false;
        }
    }

};
