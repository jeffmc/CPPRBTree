#include <cassert>
#include <cstdlib>

template <typename NT>
struct RBNode {
    typedef bool col_t;
    static constexpr col_t RED = false, BLACK = true; 

    col_t col;
    NT data;
    BNode *left = nullptr, *right = nullptr;

    BNode(const NT& o) : data(NT(o)) {}
    BNode() {}
    ~BNode() {};
};

template <typename T>
class RBTree {
    RBNode<T> *head = nullptr;
    delete_tree(RBNode<T> *x) {
        if (x == nullptr) return;
        delete_tree(x->left);
        delete_tree(x->right);
        delete x;
    }
public:
    operator bool() const { return head != nullptr; }
    template <typename ...Args> 
    // Returns true if successfully inserted, will return false if data is already contained (doesn't duplicate).
    bool insert(const T& o) {
        assert(false);
        return false;
    }
    // Delete all nodes
    void clear() 
    {
        delete_tree(head);
        head = nullptr;
    }

    template <typename NT>
    friend void print_rbt(const RBTree<NT>&);
};