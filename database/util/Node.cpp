// #include "Address.h"
// #include "LinkedList.h"

// class Node
// {
//     // int currKeyNum; // current number of keys in the node
//     // bool isLeaf; // boolean is Node a leaf
//     // Address *addressPtrs; // A pointer to an array of address -> only for leaf 
//     // Node **nodePtrs;  //A pointer to an array of Nodes -> for any internal node 
//     // float *keys; // A pointer to an array of keys
//     friend class BPlusTree;
//     //do we need to declare friend class?  yes BPlus tree need to modigy Node, and we want to keep this variable as private to public 

//     public: 
//         int currKeyNum; // current number of keys in the node
//         bool isLeaf; // boolean is Node a leaf
//         Address *addressPtrs; // A pointer to an array of address -> only for leaf 
//         Node **nodePtrs;  //A pointer to an array of Nodes -> for any internal node 
//         float *keys; // A pointer to an array of keys
//         Node(int maxKey){
//             keys = new float[maxKey];
//             currKeyNum = 0;
//         }
// };
    

#include "Node.h"
#include "Address.h"

Node::Node(int maxKey) {
    keys = new float[maxKey];
    nodePtrs = new Node*[maxKey+1];
    addressPtrs = new Address[maxKey];
    currKeyNum = 0;
};