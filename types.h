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
    BNode<T> *head = nullptr;
    void delete_tree(BNode<T> *n) {
        if (n->left) delete_tree(n->left);
        if (n->right) delete_tree(n->right);
        delete n;
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
                if (z->data <= x->data) {
                    x = x->left;
                }
                else {
                    x = x->right;
                }
            } while (x != nullptr);
            if (z->data <= y->data) {
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
    template <typename NT>
    friend void print_bst(BST<NT>& bst);
};