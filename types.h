#include <cassert>
#include <utility>

template <typename NT>
struct BNode {
    NT data;
    BNode *left = nullptr, *right = nullptr;
    BNode(NT&& o) : data(std::move(o)) {}
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
    // u is new parent, v is new child, o is orientation.
    void shift_nodes(BNode<T> *u, BNode<T> *v, ORIENTATION o)
    {
        if (u == nullptr) {
            head = v;
        }
        else if (o == LEFT) {
            u->left = v;
        }
        else {
            u->right = v;
        }
    }
    BNode<T>* bst_max(BNode<T> *x) {
        while (x->right) x = x->right;
        return x;
    }
    BNode<T>* bst_min(BNode<T> *x) {
        while (x->left) x = x->left;
        return x;
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
        BNode<T> *y = nullptr, *x = head; // y is parent of x, x is node == key.
        ORIENTATION o = LEFT; // side of y that x is on.
        while (x != nullptr && x->data != key) {
            y = x;
            if (key < x->data) {
                o = LEFT;
                x = x->left;
            }
            else {
                o = RIGHT;
                x = x->right;
            }
        }
        if (x == nullptr) return false;

        if (x->left != nullptr && x->right != nullptr) {
            // Both children
            assert(false); // TODO: THIS
        } else if (x->left != nullptr) {
            // Left child
            if (y != nullptr) {
                if (o == LEFT) {
                    y->left = x->left;
                }
                else {
                    y->right = x->left;
                }
            }
            else {
                head = x->left;
            }

        } else if (x->right != nullptr) {
            // Right child
            if (y != nullptr) {
                if (o == LEFT) {
                    y->left = x->right;
                }
                else {
                    y->right = x->right;
                }
            }
            else {
                head = x->right;
            }
        }
        else {
            // Leaf
            if (y != nullptr) {
                if (o == LEFT) {
                    y->left = nullptr;
                } else {
                    y->right = nullptr;
                }
            }
            else {
                head = nullptr;
            }
        }
        delete x;
        return true;
    }
    template <typename NT>
    friend void print_bst(const BST<NT>& bst);
};