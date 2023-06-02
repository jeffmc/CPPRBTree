#include <cassert>
#include <cstdlib>
#include <cstdint>

#include "debug.h"

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

    int child_dir() const {
        if (parent == nullptr) return -1;
        if (this == parent->left) return 0;
        if (this == parent->right) return 1;
        assert(false);
        return -2; // THIS SHOULD NEVER HAPPEN
    }

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
        
    // P is the root of subtree, dir is 0 for left, 1 for right
    RBNode<T>* rotate_dir(RBNode<T>* P, int dir) {
        RBNode<T>* G = P->parent; // grandparent of C, parent of S
        RBNode<T>* S = P->child[1-dir]; // child of P opposite the rotation
        RBNode<T>* C = S->child[dir]; // child of S in direction of rotation
        
        P->child[1-dir] = C;
        if (C != nullptr) C->parent = P;
        
        S->child[dir] = P; 
        P->parent = S;
        
        S->parent = G;
        if (G != NULL) {
            G->child[ P == G->right ? 1 : 0 ] = S;
        } else {
            head = S;
        }
        return S; // return new subtree root
    }
    RBNode<T>* rotate_left(RBNode<T>* P) {
        rotate_dir(P,0);
    }
    RBNode<T>* rotate_right(RBNode<T>* P) {
        rotate_dir(P,1);
    }

    // Idea for goto-label statements specifying different states/cases that can be 
    // returned to inherited from this Wikipedia article on RB-trees.
    // https://en.wikipedia.org/wiki/Red%E2%80%93black_tree

    // It's a kinda old-school c-style approach to this problem, but it appears to 
    // be the most concise and easiest to understand.

    // N -> node to be inserted, P -> parent node of N ( may be NULL ), byte is LEFT or RIGHT child of P to insert N
    void intl_insert(RBNode<T>* N, RBNode<T>* P, uint8_t dir)
    {
        DBGTRACE();
        // parent of P, uncle of N
        RBNode<T> *G, *U;

        N->color = color_t::RED;
        N->left  = nullptr;
        N->right = nullptr;
        N->parent = P;
        if (P == nullptr) { // inserting at root of tree
            N->color = color_t::BLACK;
            DBGLOG("Root insertion!");
            head = N;
            return;
        }
        DBGLOG("Pre-loop");
        P->child[dir] = N; // insert N as dir-child of P
        do {   
            DBGLOG("Loop iteration");
            if (P->color == color_t::BLACK) {
                Case_I1: // (P black)
                DBGLOG("Case I1");
                return; // insertion complete
            }
            // P is guaranteed red now.
            
            if ((G = P->parent) == NULL) goto Case_I4; // P red and root
            
            // else: P is red && G exists.
            dir = P->child_dir(); // the side of parent G on which node P is located
            U = G->child[1-dir]; // uncle
            if (U == nullptr || U->color == color_t::BLACK) // considered black
            goto Case_I56; // P red && U black    
            
            // Case_I2: (P+U red)
            DBGLOG("Case I2");
            P->color = color_t::BLACK;
            U->color = color_t::BLACK;
            G->color = color_t::RED;
            N = G; // iterate on the grandparent now
        } while ((P = N->parent) != NULL); 
        
        // Case_I3: N is the root and red.
        DBGLOG("Case I3");
        return; // insertion complete
        
        Case_I4: // P is the root and red
        DBGLOG("Case I4");
        P->color = color_t::BLACK;
        return; // insertion complete
        
        Case_I56: // P red && U black:
        DBGLOG("Case I56");
        if (N == P->child[1-dir])
        { 
            // Case_I5 (P red && U black && N inner grandchild of G):
            rotate_dir(P,dir); // P is never the root
            N = P; // new current node
            P = G->child[dir]; // new parent of N
        }  
        
        // Case_I6 (P red && U black && N outer grandchild of G):
        rotate_dir(G,1-dir);
        P->color = color_t::BLACK;
        G->color = color_t::RED;
        return;
    }
    void intl_delete(RBNode<T>* N)  // N -> node to be deleted
    {
        RBNode<T> *P = N->parent,  // -> parent node of N
            *S,  // Sibling of N
            *C,  // Close nephew of N
            *D;  // Distant nephew of N

        int dir = N->child_dir(); // side of P on which N is located (∈ { LEFT, RIGHT })

        // P != NULL, since N is not the root.
        
        // Replace N at its parent P by NIL:
        P->child[dir] = nullptr;
        goto Start_D;      // jump into the loop

        do {
            dir = N->child_dir();   // side of parent P on which node N is located
        Start_D:
            S = P->child[1-dir]; // sibling of N (has black height >= 1)
            D = S->child[1-dir]; // distant nephew
            C = S->child[  dir]; // close   nephew
            if (S->color == color_t::RED)
            goto Case_D3; // S red ===> P+C+D black
            // S is black:
            if (D != nullptr && D->color == color_t::RED) // not considered black
                goto Case_D6;                  // D red && S black
            if (C != nullptr && C->color == color_t::RED) // not considered black
                goto Case_D5;                  // C red && S+D black
            
            // Here both nephews are == NIL (first iteration) or black (later).
            if (P->color == color_t::RED)
                goto Case_D4; // P red && C+S+D black
            // Case_D1 (P == NULL):
            return; // deletion complete

            // Case_D2 (P+C+S+D black):
            S->color = color_t::RED;
            N = P; // new current node (maybe the root)
            
            // iterate 1 black level
            //   (= 1 tree level) higher
        } while ((P = N->parent) != nullptr);
        
        Case_D3: // S red && P+C+D black:
            rotate_dir(P,dir); // P may be the root
            P->color = color_t::RED;
            S->color = color_t::BLACK;
            S = C; // != NIL
            
            // now: P red && S black
            D = S->child[1-dir]; // distant nephew
            if (D != nullptr && D->color == color_t::RED)
                goto Case_D6;      // D red && S black

            C = S->child[dir]; // close   nephew
            if (C != nullptr && C->color == color_t::RED)
                goto Case_D5;      // C red && S+D black
            
            // Otherwise C+D considered black.
            // fall through to Case_D4
        Case_D4: // P red && S+C+D black:
            S->color = color_t::RED;
            P->color = color_t::BLACK;
            return; // deletion complete
        Case_D5: // C red && S+D black:
            rotate_dir(S,1-dir); // S is never the root
            S->color = color_t::RED;
            C->color = color_t::BLACK;
            D = S;
            S = C;
            // now: D red && S black
            // fall through to Case_D6
        Case_D6: // D red && S black:
            rotate_dir(P,dir); // P may be the root
            S->color = P->color;
            P->color = color_t::BLACK;
            D->color = color_t::BLACK;
            return; // deletion complete
    }

    const RBNode<T>* find(const T& value) const {
        const RBNode<T>* n = head;
        while (true) {
            if (n->data == value) {
                return n;
            } else if (value < n->data) {
                if (n->left == nullptr) break;
                n = n->left;
            } else if (value > n->data) {
                if (n->right == nullptr) break;
                n = n->right;
            }
        }
        return nullptr;
    }

    RBNode<T>* find(const T& value) {
        RBNode<T>* n = head;
        while (true) {
            if (n->data == value) {
                return n;
            } else if (value < n->data) {
                if (n->left == nullptr) break;
                n = n->left;
            } else if (value > n->data) {
                if (n->right == nullptr) break;
                n = n->right;
            }
        }
        return nullptr;
    }

public:
    bool contains(const T& value) const {
        return find(value) != nullptr;
    }
    operator bool() const { return head != nullptr; }

    // Returns true if successfully inserted, will return false if data is already contained (doesn't duplicate).
    bool insert(const T& o) {
        DBGTRACE();
        RBNode<T> *z = new RBNode<T>(o); // new node
        if (head == nullptr) { // empty tree case
            DBGLOG("Inserting as head");
            intl_insert(z, nullptr, 0);
            return true;
        }
        DBGLOG("Finding insertion spot in tree");
        RBNode<T>* y = nullptr, *x = head; // moving pointers (downward the tree towards an insertion target. )
        do {
            DBGLOG("Loop iteration");
            y = x;
            if (z->data == x->data) {
                delete z;
                return false;
            }
            else if (z->data < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        } while (x != nullptr);
        if (z->data < y->data) {
            intl_insert(z, y, 0); // insert z as left-child of y
        }
        else {
            intl_insert(z, y, 1); // insert z as right-child of y
        }
        return true;
    }

    // Returns true if node was successfully removed, false otherwise.
    bool remove(const T& value) {
        RBNode<T>* node = find(value);
        if (node == nullptr) return false;
        intl_delete(node);
        return true;
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