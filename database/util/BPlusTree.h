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
            if(parent!=NULL) return parent;
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

        int j=0;
        for (int j = insertPtr->currKeyNum;j > i; j--){
            insertPtr->keys[j] = insertPtr->keys[j-1];
            insertPtr->addressPtrs[j] = insertPtr->addressPtrs[j-1];
            j+=1;
        }

        insertPtr->keys[i] = key;
        insertPtr->nodePtrs[i+1] = child;
        insertPtr->currKeyNum += 1;

        std::cout << "[INSERT SUCCESS] No new node is created, key: " << key << " \n";
    }else{ //no space in leaf node
        std::cout << "[SPLITTING NODE] No space inside leaf node, key: " << key << " \n";
        Node * newNode = new Node(this->MAX_KEY);


        std::cout << "[SANITY CHECK] Number of keys in current node: " << insertPtr->currKeyNum << " \n";
        newNode->currKeyNum = insertPtr->currKeyNum - ceil((this->MAX_KEY+1) / 2) + 1;
        std::cout << "[NEW NODE COUNT] Number of keys in new node: " << newNode->currKeyNum << " \n";
        insertPtr->currKeyNum = ceil((this->MAX_KEY+1) / 2);
        std::cout << "[OLD NODE COUNT] Number of keys in old node: " << insertPtr->currKeyNum << " \n";

        //give element to new leaf nodes 
        int j = 0;
        for (int i=insertPtr->currKeyNum; i < MAX_KEY; i++){
            newNode->keys[j] = insertPtr->keys[i];
            newNode->nodePtrs[j] = insertPtr->nodePtrs[i];  //TODO: change this to address pointer once its set up
            j += 1;
        }

        //adding the new key in the new node
        newNode->keys[j] = key;

        //modify the parent
        if(insertPtr == this->root){
            //if current is already root, then we need to create a new rot
            Node * newRoot = new Node(MAX_KEY);
            newRoot->keys[0] = newNode->keys[0];
            newRoot->nodePtrs[0] = insertPtr;
            newRoot->nodePtrs[1] = newNode;
            newRoot->isLeaf = false;
            newRoot->currKeyNum = 1;
            this->root = newRoot;
            std::cout << "[CREATED NEW ROOT] \n";
        }else{
            //insert new key in parent node
            insertInternal(insertPtr->keys[insertPtr->currKeyNum], findParent(root, insertPtr), newNode);

        }
    }
    


  }   

  void print(){
    std::queue<Node*> nodesQueue;
    nodesQueue.push(this->root);

    while (!nodesQueue.empty()){
        Node * currentNode = nodesQueue.front();
        nodesQueue.pop();
        
        for (int i = 0; i < currentNode->currKeyNum; i++) {
            std::cout << currentNode->keys[i] << " ";
        }

        std::cout << "\t";

        if (!currentNode->isLeaf) {
            for (int i = 0; i <= currentNode->currKeyNum; ++i) {
                nodesQueue.push(currentNode->nodePtrs[i]);
            }
            std::cout << "\n";
        }

    }
  }
  void insert(Address address, float key){
    if(root==NULL){
            root = new Node(MAX_KEY);
            root->keys[0] = key;
            root->isLeaf = true;
            root->currKeyNum = 1;
    }else{

    Node * insertPtr = this->root;
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

        for (int j = insertPtr->currKeyNum;j > i; j--){
            insertPtr->keys[j] = insertPtr->keys[j-1];
            //insertPtr->addressPtrs[j] = insertPtr->addressPtrs[j-1];
        }
        insertPtr->keys[i] = key;
        // insertPtr->addressPtrs[i] = address; TODO: do the addressing part 
        insertPtr->currKeyNum += 1;

        std::cout << "[INSERT SUCCESS] No new node is created, key: " << key << " \n";
    }else{ //no space in leaf node
        std::cout << "[SPLITTING NODE] No space inside leaf node, key: " << key << " \n";
        Node * newNode = new Node(this->MAX_KEY);
        newNode->isLeaf = true;

        std::cout << "[SANITY CHECK] Number of keys in current node: " << insertPtr->currKeyNum << " \n";
        newNode->currKeyNum = insertPtr->currKeyNum - ceil((this->MAX_KEY+1) / 2) + 1;
        std::cout << "[NEW NODE COUNT] Number of keys in new node: " << newNode->currKeyNum << " \n";
        insertPtr->currKeyNum = ceil((this->MAX_KEY+1) / 2);
        std::cout << "[OLD NODE COUNT] Number of keys in old node: " << insertPtr->currKeyNum << " \n";

        //new node point to the next node old node was pointing to
        newNode->nodePtrs[MAX_KEY] = insertPtr->nodePtrs[MAX_KEY];
        //last pointer of the old node will point to the new node
        insertPtr->nodePtrs[MAX_KEY] = newNode;

        //give element to new leaf nodes 
        int j = 0;
        for (int i=insertPtr->currKeyNum; i < MAX_KEY; i++){
            newNode->keys[j] = insertPtr->keys[i];
            newNode->nodePtrs[j] = insertPtr->nodePtrs[i];  //TODO: change this to address pointer once its set up
            j += 1;
        }

        //adding the new key in the new node
        newNode->keys[j] = key;
        //TODO: ADD the address

        //modify the parent
        if(insertPtr == this->root){
            //if current is already root, then we need to create a new rot
            Node * newRoot = new Node(MAX_KEY);
            newRoot->keys[0] = newNode->keys[0];
            newRoot->nodePtrs[0] = insertPtr;
            newRoot->nodePtrs[1] = newNode;
            newRoot->isLeaf = false;
            newRoot->currKeyNum = 1;
            this->root = newRoot;
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
        std::cout << "[ENTERING FUNC] \n";
        Node * searchPtr;

        
        std::cout << "[STARTING SEARCH] searchKey: " << searchKey << " \n";
        searchPtr = rootPtr;

        //size = currKeyNum
        while (searchPtr->isLeaf == false){
            //traverse through each visited node
            for (int i=0; i< searchPtr->currKeyNum; i++){
                std::cout << "[SEARCHING INTERNAL NODE] Node Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                if (searchKey < searchPtr->keys[i]){
                    std::cout << "[INTERNAL NODE FOUND] < searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    searchPtr = searchPtr->nodePtrs[i];
                    break;
                }

                if (searchKey == searchPtr->keys[i]){
                    std::cout << "[INTERNAL NODE FOUND] = searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
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
                    std::cout << "[NODE FOUND] >= searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    searchPtr = searchPtr->nodePtrs[i+1];
                    break;
                }
            }  
        }

        for (int i=0; i< searchPtr->currKeyNum; i++){
            if (searchPtr->keys[i] == searchKey){
                std::cout << "[KEY FOUND] \n";
                return searchPtr;
            }
        }

        std::cout << "[KEY NOT FOUND] \n";
        return nullptr;
  };
  int findSearchKey(float searchKey){ //change the return to address once its set up
    Node * node = findCorrectNodeForKey(searchKey, this->root);
    for (int i=0; i< node->currKeyNum; i++){
        if (node->keys[i] == searchKey){
                std::cout << "[KEY FOUND] \n";
                return node->keys[i];
            }
    }
    
    std::cout << "[KEY NOT FOUND] \n";
    return -1;
  }

  
};

#endif