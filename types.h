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
public:
    operator bool() const { return head != nullptr; }
    template <typename ...Args> 
    void insert(const T& o) {
        BNode<T>* z = new BNode<T>(o);
        if (head == nullptr) {
            head = z;
        }
        else {
            BNode<T>* y = nullptr, *x = head;
            do {
                y = x;
                if (z->data < x->data) {
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
    }
    void clear() 
    {
        if (!head) return;
        delete_tree(head);
        head = nullptr;
    }
    // Returns node containing 
    BNode<T>* find(const T& key) const 
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
    bool contains(const T& key) const {
        return find(key) != nullptr;
    }
    bool remove(const T& key) const 
    {
        BNode<T> *y = nullptr, *x = head;
        ORIENTATION o = LEFT;
        while (x != nullptr && x->data != key) {
            y = x;
            if (key < x->data) {
                x = x->left;
                o = LEFT;
            }
            else {
                x = x->right;
                o = RIGHT;
            }
        }
        if (x == nullptr) return false;


    }
    template <typename NT>
    friend void print_bst(BST<NT>& bst);
};