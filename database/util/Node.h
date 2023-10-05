#ifndef NODE_H
#define NODE_H

#include "Address.h"
#include "LinkedList.h"

class Node
{
    friend class BPlusTree;

public: 
    //REMEMBER change the public member to private after testing 
    int currKeyNum; // size
    bool isLeaf; // IS_LEAF
    Address *addressPtrs; //?????
    Node **nodePtrs;  //ptr
    float *keys; // key

    Node(int maxKey);

        int getNumKeys() const {
        return currKeyNum;
    }

    bool isLeafNode() const {
        return isLeaf;
    }
};

#endif // NODE_H
