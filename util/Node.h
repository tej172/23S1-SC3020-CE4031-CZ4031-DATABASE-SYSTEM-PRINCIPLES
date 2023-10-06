#ifndef NODE_H
#define NODE_H

#include <cstddef>

template <typename T>
struct Node {
    bool is_leaf;
    std::size_t degree; // maximum number of children
    std::size_t size;   // current number of items
    T* item;
    Node<T>** children;
    Node<T>* parent;

public:
    Node(std::size_t _degree) {// Constructor
        this->is_leaf = false;
        this->degree = _degree;
        this->size = 0;

        T* _item = new T[degree - 1];
        for (std::size_t i = 0; i < degree - 1; i++) {
            _item[i] = T(); // Initialize items with default values
        }
        this->item = _item;

        Node<T>** _children = new Node<T>*[degree];
        for (std::size_t i = 0; i < degree; i++) {
            _children[i] = nullptr;
        }
        this->children = _children;

        this->parent = nullptr;
    }

    // Destructor to free allocated memory
    ~Node() {
        delete[] item;
        delete[] children;
    }
};

#endif  // NODE_H
