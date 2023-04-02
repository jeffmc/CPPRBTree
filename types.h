#include <cassert>
#include <cstdlib>

// Doubly-linked list
template<typename T>
struct DLList {
    struct DLNode {
        DLNode *prev, *next;
        T* data;
        DLNode(DLNode *prev, DLNode *next, T* data) : prev(prev), next(next), data(data) {}
        DLNode(T* data) : prev(nullptr), next(nullptr), data(data) {}
    } *head, *tail;
    operator bool() const { return head != nullptr; }

    void push_front(T* data) {
        DLNode* nn = new DLNode(tail,nullptr,data);
        tail = nn;
        if (nn->prev == nullptr) head = nn;
    }
    void push_back(T* data) {
        DLNode* nn = new DLNode(nullptr,head,data);
        head = nn;
        if (nn->next == nullptr) tail = nn;
    }
    T* pop_front() {
        DLNode* n = head;
        assert(n != nullptr);
        head = n->next;
        if (tail == n) tail = nullptr;
        T* d = n->data;
        delete n;
        return d;
    }
    T* pop_back() {
        DLNode* n = tail;
        assert(n != nullptr);
        tail = n->prev;
        if (head == n) head = nullptr;
        T* d = n->data;
        delete n;
        return d;
    }
};

enum class color_t { RED, BLACK };

template <typename NT>
struct RBNode {
    color_t color = color_t::RED;
    NT data;
    RBNode *parent = nullptr;
    static constexpr char LEFT = 0, RIGHT = 1;
    union {
        RBNode* child[2] = { nullptr, nullptr }; // 0th is left, 1st is right
        struct { RBNode *left, *right; };
    };

    RBNode(const NT& o) : data(NT(o)) {}
    ~RBNode() {};
};

template<typename T>
static void validate_tree(RBNode<T> *n) {
    if (n == nullptr) return;
    RBNode<T> *p = nullptr;
    assert(n->parent == p); // p's children don't reference p as their parent.
    // TODO: add validation for red-black coloring.
    validate_tree(n->left);
    validate_tree(n->right);
}

template<typename T>
static void delete_tree(RBNode<T> *x) {
    if (x == nullptr) return;
    delete_tree(x->left);
    delete_tree(x->right);
    delete x;
}

template <typename T>
class RBTree {
    RBNode<T> *head = nullptr;
public:
    operator bool() const { return head != nullptr; }
    template <typename ...Args> 
    // Returns true if successfully inserted, will return false if data is already contained (doesn't duplicate).
    bool insert(const T& o) {

        return false;
    }
    // Validate tree structure
    void validate() {
        validate_tree(head);
    }
    // Delete all nodes
    void clear() {
        delete_tree(head);
        head = nullptr;
    }

    friend void print_rbt(const RBTree<int>&);
    friend void example_rbtree(RBTree<int>&);
};

// RBNode
#define RBN(V) RBNode<int> * n ## V = new RBNode(V);
// Parent - left - right, triangle relationship
#define PLR(P,L,R) n ## P->left = n ## L; n ## P->right = n ## R; n ## L->parent = n ## P; n ## R->parent = n ## P; 
// Color a node black 
#define BLACK(V) n ## V ->color = color_t::BLACK;

void example_rbtree(RBTree<int>& rbt) {
    rbt.clear();
    RBN(13);
    RBN(8);
    RBN(17);
    RBN(1);
    RBN(11);
    RBN(15);
    RBN(25);
    RBN(6);
    RBN(22);
    RBN(27);
    
    BLACK(13);
    BLACK(1);
    BLACK(11);
    BLACK(15);
    BLACK(25);

    PLR(13,8,17);
    PLR(8,1,11);
    PLR(17,15,25);
    PLR(25,22,27);

    n1->right = n6;
    n6->parent = n1;

    rbt.head = n13;
}

#undef RBN
#undef PLR