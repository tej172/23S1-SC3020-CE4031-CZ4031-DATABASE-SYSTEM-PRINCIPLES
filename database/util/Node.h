#ifndef NODE_H
#define NODE_H

#include "Address.h"
#include "LinkedList.h"

class Node
{
    friend class BPlusTree;

public: 
    //REMEMBER change the public member to private after testing 
    int currKeyNum; // current number of keys in the node
    bool isLeaf; // boolean is Node a leaf
    Address *addressPtrs; // A pointer to an array of address -> only for leaf 
    Node **nodePtrs;  //A pointer to an array of Nodes -> for any internal node 
    float *keys; // A pointer to an array of keys

    Node(int maxKey);
};

#endif // NODE_H
