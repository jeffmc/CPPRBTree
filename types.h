#include <cassert>
#include <cstdlib>

template <typename NT>
struct BNode {
    NT data;
    BNode *left = nullptr, *right = nullptr;
    BNode(const NT& o) : data(NT(o)) {}
    BNode() {}
    ~BNode() {};
};

template <typename T>
class BST {
    typedef bool ORIENTATION;
    static const ORIENTATION LEFT = false, RIGHT = true;
    BNode<T> *head = nullptr;
    void delete_tree(BNode<T> *n) {
        if (n->left) delete_tree(n->left);
        if (n->right) delete_tree(n->right);
        delete n;
    }
    void shift_nodes(BNode<T>* up, BNode<T> *u, BNode<T> *v) {
        if (up == nullptr) {
            assert(u == head);
            head = v;
        } else if (u == up->left) {
            up->left = v;
        } else {
            up->right = v;
        }
    } 
    BNode<T>* bst_minimum(BNode<T> *x) {
        while (x->left != nullptr) x = x->left;
        return x;
    } 
    BNode<T>* bst_minimum(BNode<T> *x, BNode<T>* &p) {
        while (x->left != nullptr) {
            p = x;
            x = x->left;
        }
        return x;
    }
    BNode<T>* bst_maximum(BNode<T> *x) {
        while (x->right != nullptr) x = x->right;
        return x;
    }
    // Parent list, parent list size
    BNode<T>* successor(BNode<T>** pl, int plsz, BNode<T> *x) {
        if (x->right != nullptr) {
            return bst_minimum(x->right);
        }
        BNode<T> *y = x->parent;
        while (y != nullptr && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }
    // Parent list, parent list size
    BNode<T>* predecessor(BNode<T>** pl, int plsz, BNode<T> *x) {
        if (x->left != nullptr) {
            return bst_maximum(x->left);
        }
        BNode<T> *y = x->parent;
        while (y != nullptr && x == y->left) {
            x = y;
            y = y->parent;
        }
        return y;
    }
    void bst_delete(BNode<T>* dp, BNode<T> *d) {
        if (d->left == nullptr) {
            shift_nodes(dp, d, d->right);
        } else if (d->right == nullptr) {
            shift_nodes(dp, d, d->left);
        } else {
            BNode<T> *ep = nullptr;
            BNode<T> *e = bst_minimum(d->right, ep);
            if (e != d->right) { // if successor is not d's immediate child
                shift_nodes(ep, e, e->right);
                e->right = d->right;
            }
            shift_nodes(dp,d,e);
            e->left = d->left;
        }
        delete d;
    }
public:
    operator bool() const { return head != nullptr; }
    template <typename ...Args> 
    // Returns true if not already contained (doesn't duplicate), insert the key at a fitting space in the tree.
    bool insert(const T& o) {
        BNode<T>* z = new BNode<T>(o);
        if (head == nullptr) {
            head = z;
        }
        else {
            BNode<T>* y = nullptr, *x = head;
            do {
                y = x;
                if (z->data == x->data) {
                    delete z;
                    return true;
                }
                else if (z->data < x->data) {
                    x = x->left;
                }
                else {
                    x = x->right;
                }
            } while (x != nullptr);
            if (z->data < y->data) {
                y->left = z;
            }
            else {
                y->right = z;
            }
        }
        return false;
    }
    // Delete all nodes
    void clear() 
    {
        if (!head) return;
        delete_tree(head);
        head = nullptr;
    }
    // Returns (first) node equal to specified key
    BNode<T>* find(const T& key) 
    {
        BNode<T> *x = head;
        while (x != nullptr && x->data != key) {
            if (key < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        return x;
    }
    // Returns (first) node equal to specified key
    const BNode<T>* find(const T& key) const 
    {
        BNode<T> *x = head;
        while (x != nullptr && x->data != key) {
            if (key < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        return x;
    }
    // Return true if BST contains key (at least once).
    bool contains(const T& key) const {
        return find(key) != nullptr;
    }
    // Return true if key was removed from BST (once).
    bool remove(const T& key)  
    {
        BNode<T> *xp = nullptr, *x = head;
        while (x != nullptr && x->data != key) {
            xp = x;
            if (key < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        if (x == nullptr) return false;
        bst_delete(xp,x);
        return true;
    }
    template <typename NT>
    friend void print_bst(const BST<NT>& bst);
};