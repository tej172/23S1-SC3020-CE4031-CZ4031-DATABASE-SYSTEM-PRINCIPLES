#include "Node.cpp"

#ifndef BPLUSTREEFIND_H
#define BPLUSTREEFIND_H


#include <iostream>
#include <cmath>
#include <queue>

class BPlusTree
{
// private:
//   Node *root;       
//   int MAX_KEY;   
//   void insertInternal(float key, Node *cursorDiskAddress, Node *childDiskAddress);

public:
  Node *root;       
  int MAX_KEY;   
  BPlusTree(int MAX_KEY){
    
    this->root = nullptr;
    this->MAX_KEY = MAX_KEY; 
  }

  Node * findParent(Node * searchPtr, Node * child){
    //ignores second last level because we will never find parent of a leaf node during insertion using this function
    Node* parent;
    if(searchPtr->isLeaf || searchPtr->nodePtrs[0]->isLeaf){
        return NULL;
    }

    for (int i = 0; i < searchPtr->currKeyNum+1; i++){
        if(searchPtr->nodePtrs[i] == child){
            parent = searchPtr;
            return parent;
        }else{
            parent = findParent(searchPtr->nodePtrs[i],child);
            if(parent!=NULL)
                return parent;
        }
    }
    return parent;

  }

  void insertInternal(float key, Node * insertPtr, Node * child){
    std::cout << "[START INSERT INTERNAL] \n";

    //If there's still space in the leaf node
    if (insertPtr->currKeyNum < MAX_KEY){
        std::cout << "[STARTING INSERTION] No new node is created, key: " << key << " \n";
        int i = 0;
        //by right no need to check again? since its already checked by the if condition?
        while(key > insertPtr->keys[i] && i < insertPtr->currKeyNum) i++;

        for (int j = insertPtr->currKeyNum; j > i; j--)
            insertPtr->keys[j] = insertPtr->keys[j - 1];
        for (int j = insertPtr->currKeyNum + 1; j > i + 1; j--)
            insertPtr->nodePtrs[j] = insertPtr->nodePtrs[j - 1];

        insertPtr->keys[i] = key;
        insertPtr->nodePtrs[i+1] = child;
        insertPtr->currKeyNum += 1;

        std::cout << "[INSERT SUCCESS] No new node is created, key: " << key << " \n";
    }else{ //no space in leaf node
        std::cout << "[SPLITTING NODE] No space inside leaf node, key: " << key << " \n";
        Node * newNode = new Node(MAX_KEY);
        
        Node * tempNode = new Node(MAX_KEY+1);
        //storing curr key in temp key can use max key, because we kno if go here then curr node is full
        for (int i = 0; i < MAX_KEY; i++)
            tempNode->keys[i] = insertPtr->keys[i];
 
        for (int i = 0; i < MAX_KEY + 1; i++)
            tempNode->nodePtrs[i] = insertPtr->nodePtrs[i];

        //find where key has to be inserted
        int i = 0;
        while (key > tempNode->keys[i] && i < MAX_KEY) i++;
        
        //shift the key to make space for the key to be inserted
        for (int j = MAX_KEY + 1; j > i; j--)
            tempNode->keys[j] = tempNode->keys[j - 1];
        tempNode->keys[i] = key;
        for (int j = MAX_KEY + 2; j > i + 1; j--)
            tempNode->nodePtrs[j] = tempNode->nodePtrs[j - 1];
        tempNode->nodePtrs[i+1] = child;
        newNode->isLeaf = false;
        
        //start splitting the node
        insertPtr->currKeyNum = (MAX_KEY+1)/2;
        newNode->currKeyNum = MAX_KEY - (MAX_KEY+1)/2;

        for (int i = 0, j = insertPtr->currKeyNum + 1; i < newNode->currKeyNum; i++, j++)
            newNode->keys[i] = tempNode->keys[j];
        for (int i = 0, j = insertPtr->currKeyNum + 1; i < newNode->currKeyNum + 1; i++, j++)
            newNode->nodePtrs[i] = tempNode->nodePtrs[j];

        //modify the parent
        if(insertPtr == root){
            //if current is already root, then we need to create a new rot
            Node * newRoot = new Node(MAX_KEY);
            newRoot->keys[0] = insertPtr->keys[insertPtr->currKeyNum];
            newRoot->nodePtrs[0] = insertPtr;
            newRoot->nodePtrs[1] = newNode;
            newRoot->isLeaf = false;
            newRoot->currKeyNum = 1;
            root = newRoot;
            std::cout << "[CREATED NEW ROOT] \n";
        }else{
            //insert new key in parent node
            insertInternal(insertPtr->keys[insertPtr->currKeyNum], findParent(root, insertPtr), newNode);

        }
    }
    


  }   

  void print(Node * root){ 
    try{
   if (root == NULL)
        return;
    std::queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        int l;
        l = q.size();
 
        for (int i = 0; i < l; i++) {
            Node* tNode = q.front();
            q.pop();
 
            for (int j = 0; j < tNode->currKeyNum; j++)
                if (tNode != NULL)
                    std::cout << tNode->keys[j] << " ";
 
            for (int j = 0; j < tNode->currKeyNum + 1; j++)
                if (tNode->nodePtrs[j] != NULL)
                    q.push(tNode->nodePtrs[j]);
 
            std::cout << "\t";
        }
        std::cout << std::endl;
    }}catch(const std::exception& e){

    }
  }
  
  void insert(Address address, float key){
    if(root==NULL){
            root = new Node(MAX_KEY);
            root->keys[0] = key;
            root->isLeaf = true;
            root->currKeyNum = 1;
    }else{

    Node * insertPtr = root;
    Node * parent;

    std::cout << "[STARTING SEARCH] key: " << key << " \n";
    while(insertPtr->isLeaf == false){
        parent = insertPtr;
        for (int i=0; i < insertPtr->currKeyNum; i++){
            std::cout << "[SEARCHING INTERNAL NODE] \n";
            if (key <= insertPtr->keys[i]){
                std::cout << "[INTERNAL NODE FOUND] <= key. Node Key: " << insertPtr->keys[i] << ", key: " << key << " \n";
                insertPtr = insertPtr->nodePtrs[i];
                break;
            }

            if (i == insertPtr->currKeyNum - 1){
                std::cout << "[INTERNAL NODE FOUND] > key. Node Key: " << insertPtr->keys[i] << ", key: " << key << " \n";
                insertPtr = insertPtr->nodePtrs[i+1];
                break;
            }
        }
    }

    //If there's still space in the leaf node
    if (insertPtr->currKeyNum < this->MAX_KEY){
        std::cout << "[STARTING INSERTION] No new node is created, key: " << key << " \n";
        int i = 0;
        //by right no need to check again? since its already checked by the if condition?
        while(key > insertPtr->keys[i] && i < insertPtr->currKeyNum) i++;

        for (int j = insertPtr->currKeyNum; j > i; j--){
            insertPtr->keys[j] = insertPtr->keys[j-1];
            //insertPtr->addressPtrs[j] = insertPtr->addressPtrs[j-1];
        }
        insertPtr->keys[i] = key;
        // insertPtr->addressPtrs[i] = address; TODO: do the addressing part 
        insertPtr->currKeyNum += 1;
        
        //??????
        insertPtr->nodePtrs[insertPtr->currKeyNum] = insertPtr->nodePtrs[insertPtr->currKeyNum - 1];
        insertPtr->nodePtrs[insertPtr->currKeyNum - 1] = NULL;

        std::cout << "[INSERT SUCCESS] No new node is created, key: " << key << " \n";
    }else{ //no space in leaf node
        std::cout << "[SPLITTING NODE] No space inside leaf node, key: " << key << " \n";
        Node * newNode = new Node(MAX_KEY);
        Node * tempNode = new Node(MAX_KEY+1);

        for (int i = 0; i < MAX_KEY; i++)
            // all elements of this block stored
            tempNode->keys[i] = insertPtr->keys[i];

        int i = 0;
        while (key > tempNode->keys[i] && i < MAX_KEY) i++;

        for (int j = MAX_KEY + 1; j > i; j--)
            tempNode->keys[j] = tempNode->keys[j - 1];
        // inserted element in its rightful position;
        tempNode->keys[i] = key;

        newNode->isLeaf = true;
        insertPtr->currKeyNum = (MAX_KEY + 1) / 2;
        newNode->currKeyNum = (MAX_KEY + 1) - (MAX_KEY + 1) / 2;
        
        // now rearrangement begins!

        insertPtr->nodePtrs[insertPtr->currKeyNum] = newNode;
        newNode->nodePtrs[newNode->currKeyNum] = insertPtr->nodePtrs[MAX_KEY];
 
        insertPtr->nodePtrs[newNode->currKeyNum] = insertPtr->nodePtrs[MAX_KEY];
        insertPtr->nodePtrs[MAX_KEY] = NULL;
 
        for (int i = 0; i < insertPtr->currKeyNum; i++)
            insertPtr->keys[i] = tempNode->keys[i];

        for (int i = 0, j = insertPtr->currKeyNum; i < newNode->currKeyNum; i++, j++)
            newNode->keys[i] = tempNode->keys[j];

        //modify the parent
        if(insertPtr == root){
            //if current is already root, then we need to create a new rot
            Node * newRoot = new Node(MAX_KEY);
            newRoot->keys[0] = newNode->keys[0];
            newRoot->nodePtrs[0] = insertPtr;
            newRoot->nodePtrs[1] = newNode;
            newRoot->isLeaf = false;
            newRoot->currKeyNum = 1;
            root = newRoot;
            std::cout << "[CREATED NEW ROOT] \n";
        }else{
            //insert new key in parent node
            insertInternal(newNode->keys[0], parent, newNode);

        }
    }
    }
  }
  Node * findCorrectNodeForKey(float searchKey, Node * rootPtr = nullptr){
        //std::cout << "[STARTING SEARCH] searchKey: " << searchKey << " \n";
        //std::cout << "[ENTERING FUNC] \n";
        Node * searchPtr;

        
        //std::cout << "[STARTING SEARCH] searchKey: " << searchKey << " \n";
        searchPtr = rootPtr;

        //size = currKeyNum
        while (searchPtr->isLeaf == false){
            //traverse through each visited node
            for (int i=0; i< searchPtr->currKeyNum; i++){
                //std::cout << "[SEARCHING INTERNAL NODE] Node Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                if (searchKey < searchPtr->keys[i]){
                    //std::cout << "[INTERNAL NODE FOUND] < searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    searchPtr = searchPtr->nodePtrs[i];
                    break;
                }

                if (searchKey == searchPtr->keys[i]){
                    //std::cout << "[INTERNAL NODE FOUND] = searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    //need to search for left and right node 
                    Node * leftNode = searchPtr->nodePtrs[i];
                    Node * rightNode = searchPtr->nodePtrs[i+1];

                    leftNode = findCorrectNodeForKey(searchKey, leftNode);
                    rightNode = findCorrectNodeForKey(searchKey, rightNode);

                    searchPtr = (leftNode == nullptr && rightNode == nullptr) ? nullptr :
                                (leftNode != nullptr && rightNode == nullptr) ? leftNode :
                                (leftNode == nullptr && rightNode != nullptr) ? rightNode :
                                leftNode;
                    break;
                }

                if (i == searchPtr->currKeyNum - 1){
                    //std::cout << "[NODE FOUND] >= searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    searchPtr = searchPtr->nodePtrs[i+1];
                    break;
                }
            }  
        }

        for (int i=0; i< searchPtr->currKeyNum; i++){
            if (searchPtr->keys[i] == searchKey){
                //std::cout << "[KEY FOUND] \n";
                return searchPtr;
            }
        }

        //std::cout << "[KEY NOT FOUND] \n";
        return nullptr;
  };
  int findSearchKey(float searchKey){ //change the return to address once its set up
    Node * node = findCorrectNodeForKey(searchKey, this->root);
    if (node != nullptr){
    for (int i=0; i< node->currKeyNum; i++){
        if (node->keys[i] == searchKey){
                std::cout << "[KEY FOUND] \n";
                return node->keys[i];
            }
    }}
    
    std::cout << "[KEY NOT FOUND] \n";
    return -1;
  }
  void deleteNode(float key)
  {
    if(root == NULL){
        std::cout << "[ERROR] Tree is empty. \n";
        return;
    }else{

    }
  }
};

#endif