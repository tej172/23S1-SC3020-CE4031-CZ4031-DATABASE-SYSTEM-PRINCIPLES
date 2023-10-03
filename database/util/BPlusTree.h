#include "Node.cpp"

#ifndef BPLUSTREEFIND_H
#define BPLUSTREEFIND_H


#include <iostream>
#include <climits>
#include <fstream>
#include <cmath>
#include <sstream>
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
            root->addressPtrs[0] = address; 
            root->keys[0] = key;
            root->isLeaf = true;
            root->currKeyNum = 1;
    }else{
    //cursor == insertPtr
    //parent == parent
    //size = currKeyNum
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
            insertPtr->addressPtrs[j] = insertPtr->addressPtrs[j-1];
        }
        insertPtr->keys[i] = key;
        insertPtr->addressPtrs[i] = address; 
        insertPtr->currKeyNum += 1;
        
        // ensure that the leaf nodes remain linked together in the correct order after inserting a new key into the B+ tree
        insertPtr->nodePtrs[insertPtr->currKeyNum] = insertPtr->nodePtrs[insertPtr->currKeyNum - 1];
        insertPtr->nodePtrs[insertPtr->currKeyNum - 1] = NULL;

        std::cout << "[INSERT SUCCESS] No new node is created, key: " << key << " \n";
    }else{ //no space in leaf node
        std::cout << "[SPLITTING NODE] No space inside leaf node, key: " << key << " \n";
        Node * newNode = new Node(MAX_KEY);
        Node * tempNode = new Node(MAX_KEY+1);

        for (int i = 0; i < MAX_KEY; i++){
            // all elements of this block stored
            tempNode->keys[i] = insertPtr->keys[i];
            tempNode->addressPtrs[i] = insertPtr->addressPtrs[i];
        }
            

        int i = 0;
        while (key > tempNode->keys[i] && i < MAX_KEY) i++;

        for (int j = MAX_KEY + 1; j > i; j--){
            tempNode->keys[j] = tempNode->keys[j - 1];
            tempNode->addressPtrs[j] = tempNode->addressPtrs[j];
        }
        // inserted element in its rightful position;
        tempNode->keys[i] = key;
        tempNode->addressPtrs[i] = address;

        newNode->isLeaf = true;
        insertPtr->currKeyNum = (MAX_KEY + 1) / 2;
        newNode->currKeyNum = (MAX_KEY + 1) - (MAX_KEY + 1) / 2;
        
        // now rearrangement begins!

        insertPtr->nodePtrs[insertPtr->currKeyNum] = newNode;
        newNode->nodePtrs[newNode->currKeyNum] = insertPtr->nodePtrs[MAX_KEY];
 
        insertPtr->nodePtrs[newNode->currKeyNum] = insertPtr->nodePtrs[MAX_KEY];
        insertPtr->nodePtrs[MAX_KEY] = NULL;
 
        for (int i = 0; i < insertPtr->currKeyNum; i++){
            insertPtr->keys[i] = tempNode->keys[i];
            insertPtr->addressPtrs[i] = tempNode->addressPtrs[i];
        }
            

        for (int i = 0, j = insertPtr->currKeyNum; i < newNode->currKeyNum; i++, j++){
            newNode->keys[i] = tempNode->keys[j];
            newNode->addressPtrs[i] = tempNode->addressPtrs[j];
        }

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
  Address * findSearchKey(float searchKey){ //change the return to address once its set up
    Node * node = findCorrectNodeForKey(searchKey, this->root);
    if (node != nullptr){
    for (int i=0; i< node->currKeyNum; i++){
        if (node->keys[i] == searchKey){
                std::cout << "[KEY FOUND] \n";
                Address* addressPointer = &node->addressPtrs[i];
                return addressPointer;
            }
    }}
    
    std::cout << "[KEY NOT FOUND] \n";
    return nullptr;
  }

  void printTree(Node *displayNode){
    std::cout << "===============================\n";
    std::cout << "B+ Tree Print Out\n";
    std::cout << "===============================\n";

    displayBT(displayNode);
  }

  void displayBT(Node *displayNode){
    //display(root);
    if(displayNode != NULL){
        for(int i = 0; i < displayNode->currKeyNum; i++){
            std::cout << displayNode->keys[i] << " ";
        }
        std::cout << "\n";
        if(displayNode->isLeaf == false){
            for(int i = 0; i < displayNode->currKeyNum + 1; i++){
                displayBT(displayNode->nodePtrs[i]);
            }
        }
    }
    std::cout << "===============================\n";
  }
  void deleteNode(float key)
  {
    if(root == NULL){
        std::cout << "[ERROR] Tree is empty. \n";
        return;
    }
    else{
        Node *currNode = root;
        Node *parent;

        int leftSibling, rightSibling;

        while(currNode->isLeaf!=true)
        {
            //parent = currNode; //modi_1
            for(int i = 0; i < currNode->currKeyNum; i++)
            {
                parent = currNode; // modi_1
                leftSibling = i-1;
                rightSibling = i+1;
                if(key < currNode->keys[i])
                {
                    currNode = currNode->nodePtrs[i];
                    break;
                }
                if(i == (currNode->currKeyNum - 1))
                {
                    //added in_Start
                    leftSibling = i;
                    rightSibling=i+2;
                    // added in_End
                    currNode = currNode->nodePtrs[i+1];
                    break;
                }
            }
        }
        
        bool flagFound = false;
        int leafPosition;
        for (leafPosition = 0; leafPosition < currNode->currKeyNum; leafPosition++)
        {
            if (currNode->keys[leafPosition] == key)
            {
                flagFound = true;
                break;
            }
        }
        if(flagFound==false){
            std::cout << "==================================\n";
            std::cout << "[KEY NOT FOUND]: Nothing to delete in tree";
            std::cout << "\n==================================\n";
            return;
        }else{
            // std::cout<< "=========\nfound key";
            // std::cout<< "\nkey: " << key << "\nleaf position: "<< leafPosition <<"\n=========\n";
            std::cout << "=========\nbefore delete\n=========\n";

            for(int i=leafPosition; i<currNode->currKeyNum;i++){
                currNode->keys[i] = currNode->keys[i+1];
                //currNode->addressPtrs[i] = currNode->addressPtrs[i+1];
            }
            currNode->currKeyNum -= 1;
            std::cout << "=========\nafter delete\n=========\n";
            if(currNode==root){
                //if no keys left (i.e. all keys deleted)
                //cout<<"Deleted"
                for(int i=0; i<currNode->currKeyNum; i++){
                    currNode->nodePtrs[i] =NULL;
                    //cout<<"Address of node["<<i<<"] is :"<<currNode->addressPtrs[i]->pageNo <<" ";
                }
                if(currNode->currKeyNum==0)
                {
                    //modi2 (switch)
                    delete[] currNode->nodePtrs;
                    delete[] currNode->keys;
                    delete currNode;
                    std::cout << "=========\ntree DELETED\n=========\n";
                }
                return; //tree deleted                        
            }
            currNode->nodePtrs[currNode->currKeyNum] = currNode->nodePtrs[currNode->currKeyNum+1];
            currNode->nodePtrs[currNode->currKeyNum+1] = NULL;
            if(currNode->currKeyNum >= (MAX_KEY+1)/2){
                std::cout << "=========\nno underflow\n=========\n";
                return;
            }

            if(leftSibling >=0){
                Node *leftNode = parent->nodePtrs[leftSibling];
                if(leftNode->currKeyNum>=((MAX_KEY+1)/2+1)){
                    for(int i = currNode->currKeyNum; i>0; i--){
                        currNode->keys[i] = currNode->keys[i-1];
                        //currNode->addressPtrs[i] = currNode->addressPtrs[i-1];
                    }
                    currNode->currKeyNum++;
                    currNode->nodePtrs[currNode->currKeyNum] = currNode->nodePtrs[currNode->currKeyNum-1];
                    currNode->nodePtrs[currNode->currKeyNum -1] =NULL;
                    currNode->keys[0] = leftNode->keys[leftNode->currKeyNum -1];
                    //currNode->addressPtrs[0] = leftNode->addressPtrs[leftNode->currKeyNum -1];
                    leftNode->currKeyNum--;
                    leftNode->nodePtrs[leftNode->currKeyNum]=currNode;
                    leftNode->nodePtrs[leftNode->currKeyNum + 1]=NULL;
                    parent->keys[leftSibling] = currNode->keys[0];
                    return;
                }
            }
            if (rightSibling <= parent->currKeyNum){
                Node *rightNode = parent->nodePtrs[rightSibling];
                if(rightNode->currKeyNum >= (((MAX_KEY+1)/2)+1)){
                    currNode->currKeyNum++;

                    currNode->nodePtrs[currNode->currKeyNum] = currNode->nodePtrs[currNode->currKeyNum -1];
                    currNode->nodePtrs[currNode->currKeyNum -1] = NULL;
                    currNode->keys[currNode->currKeyNum - 1] = rightNode->keys[0];
                    //currNode->addressPtrs[currNode->currKeyNum -1] = rightNode->addressPtrs[0];
                    rightNode->currKeyNum--;

                    rightNode->nodePtrs[rightNode->currKeyNum] =rightNode->nodePtrs[rightNode->currKeyNum +1];
                    rightNode->nodePtrs[rightNode->currKeyNum +1] = NULL;
                    // currNode->nodePtrs[currNode->currKeyNum] = rightNode->nodePtrs[0];
                    // rightNode->nodePtrs[0] = rightNode->nodePtrs[1];
                    // rightNode->nodePtrs[rightNode->currKeyNum] = NULL;

                    for(int i=0; i<rightNode->currKeyNum; i++){
                        rightNode->keys[i] = rightNode->keys[i+1];
                        //rightNode->addressPtrs[i] = rightNode->addressPtrs[i+1];
                    }
                    // rightNode->currKeyNum--;
                    parent->keys[rightSibling-1] = rightNode->keys[0];
                    return;
                }
            }
            if(leftSibling>=0){
                Node *leftNode =parent->nodePtrs[leftSibling];
                for(int i=leftNode->currKeyNum, j=0; j<currNode->currKeyNum; i++, j++){
                    leftNode->keys[i] = currNode->keys[j];
                    //leftNode->addressPtrs[i] = currNode->addressPtrs[j];
                }
                leftNode->nodePtrs[leftNode->currKeyNum] = NULL;
                leftNode->currKeyNum += currNode->currKeyNum;
                leftNode->nodePtrs[leftNode->currKeyNum] = currNode->nodePtrs[currNode->currKeyNum];
                //delete currNode;
                deleteInternal(parent->keys[leftSibling], parent, currNode);
                delete[] currNode->keys;
                delete[] currNode->nodePtrs;
                delete currNode;
            }else if(rightSibling <= parent->currKeyNum){
                Node *rightNode =  parent->nodePtrs[rightSibling];
                for(int i =currNode->currKeyNum, j=0; j<rightNode->currKeyNum; i++,j++){
                    currNode->keys[i] = rightNode->keys[j];
                    //currNode->addressPtrs[i] = rightNode->addressPtrs[j];
                }
                currNode->nodePtrs[currNode->currKeyNum] =NULL;
                currNode->currKeyNum += rightNode->currKeyNum;
                currNode->nodePtrs[currNode->currKeyNum]=rightNode->nodePtrs[rightNode->currKeyNum];
                //delete rightNode;
                std::cout << "==============\nMerging Leaf Node\n==============\n";
                deleteInternal(parent->keys[rightSibling-1], parent, rightNode);
                delete[] rightNode->keys;
                delete[] rightNode->nodePtrs;
                delete rightNode;
            }
        }
    }
  }
  void deleteInternal(float key, Node *currNode, Node *child){ //touch-upcode
    if (currNode == root)
    {
        if (currNode->currKeyNum == 1)
        {
            if (currNode->nodePtrs[1] == child)
            {
                delete[] child->keys;
                delete[] child->nodePtrs;
                delete child;


                root = currNode->nodePtrs[0];
                delete[] currNode->keys;
                delete[] currNode->nodePtrs;

                std::cout << "===============================\nRoot Node Has Been Changed\n===============================\n";
                delete currNode;
                
                return;
            }
            else if (currNode->nodePtrs[0] == child)
            {
                delete[] child->keys;
                delete[] child->nodePtrs;
                delete child;
                root = currNode->nodePtrs[1];
                delete[] currNode->keys;
                delete[] currNode->nodePtrs;

                std::cout << "===============================\nRoot Node Has Been Changed\n===============================\n";
                delete currNode;
                
                
                return;
            }
        }
    }
    int position;
    for (position = 0; position < currNode->currKeyNum; position++){
        if (currNode->keys[position] == key){
            break;
        }
    }
    for (int i = position; i < currNode->currKeyNum; i++){
        currNode->keys[i] = currNode->keys[i + 1];
    }
    for (position = 0; position < currNode->currKeyNum + 1; position++){
        if (currNode->nodePtrs[position] == child){
            break;
        }
    }
    for (int i = position; i < currNode->currKeyNum + 1; i++){
        currNode->nodePtrs[i] = currNode->nodePtrs[i + 1];
    }

    //decrement current node num of keys 
    currNode->currKeyNum--;

    if (currNode->currKeyNum >= (MAX_KEY + 1) / 2 - 1){
        return;
    }

    if (currNode == root){
        return;
    }
        
    Node *parent = findParent(root, currNode);
    int leftSibling, rightSibling;

    // for (position = 0; position < parent->currKeyNum + 1; position++){
    //     if (parent->nodePtrs[position] == currNode){
    //         leftSibling = position - 1;
    //         rightSibling = position + 1;
    //         break;
    //     }
    // }
    //if (!isLeftChild(parent)){

    for (position = 0; position < parent->currKeyNum + 1; position++){
        if (parent->nodePtrs[position] == currNode){
            leftSibling = position - 1;
            rightSibling = position + 1;
            break;
        }
    }
    if (leftSibling >= 0){
        Node *leftNode = parent->nodePtrs[leftSibling];

        if (leftNode->currKeyNum >= (MAX_KEY + 1) / 2){
            for (int i = currNode->currKeyNum; i > 0; i--){
                currNode->keys[i] = currNode->keys[i - 1];
            }
            currNode->keys[0] = parent->keys[leftSibling];
            parent->keys[leftSibling] = leftNode->keys[leftNode->currKeyNum - 1];
            for (int i = currNode->currKeyNum + 1; i > 0; i--){
                currNode->nodePtrs[i] = currNode->nodePtrs[i - 1];
            }
            currNode->nodePtrs[0] = leftNode->nodePtrs[leftNode->currKeyNum];
            currNode->currKeyNum++;
            leftNode->currKeyNum--;
            //updateHeightAndBalanceFactor(currNode);//update height and balance factor
            
            return;
        }
    }
    if (rightSibling <= parent->currKeyNum)
    {
        Node *rightNode = parent->nodePtrs[rightSibling];
        if (rightNode->currKeyNum >= (MAX_KEY + 1) / 2){

            currNode->keys[currNode->currKeyNum] = parent->keys[position];
            parent->keys[position] = rightNode->keys[0];

            for (int i = 0; i < rightNode->currKeyNum - 1; i++){
                rightNode->keys[i] = rightNode->keys[i + 1];
            }

            currNode->nodePtrs[currNode->currKeyNum + 1] = rightNode->nodePtrs[0];
            
            for (int i = 0; i < rightNode->currKeyNum; ++i){
                rightNode->nodePtrs[i] = rightNode->nodePtrs[i + 1];
            }
            currNode->currKeyNum++;
            rightNode->currKeyNum--;
            return;
        }
    }
    if (leftSibling >= 0)
    {
        Node *leftNode = parent->nodePtrs[leftSibling];
        leftNode->keys[leftNode->currKeyNum] = parent->keys[leftSibling];
        for (int i = leftNode->currKeyNum + 1, j = 0; j < currNode->currKeyNum; j++){
            leftNode->keys[i] = currNode->keys[j];
        }
        for (int i = leftNode->currKeyNum + 1, j = 0; j < currNode->currKeyNum + 1; j++){
            leftNode->nodePtrs[i] = currNode->nodePtrs[j];
            currNode->nodePtrs[j] = NULL;
        }

        leftNode->currKeyNum += currNode->currKeyNum + 1;
        currNode->currKeyNum = 0;
        deleteInternal(parent->keys[leftSibling], parent, currNode);
    }
    else if (rightSibling <= parent->currKeyNum)
    {
        Node *rightNode = parent->nodePtrs[rightSibling];
        //cout << "right node: "; 
        //printKeys(rightNode);cout<<endl;
        currNode->keys[currNode->currKeyNum] = parent->keys[rightSibling - 1];
        for (int i = currNode->currKeyNum + 1, j = 0; j < rightNode->currKeyNum; j++){
            currNode->keys[i] = rightNode->keys[j];
        }
        for (int i = currNode->currKeyNum + 1, j = 0; j < rightNode->currKeyNum + 1; j++)
        {
            currNode->nodePtrs[i] = rightNode->nodePtrs[j];
            rightNode->nodePtrs[j] = NULL;
        }
        currNode->currKeyNum += rightNode->currKeyNum + 1;
        rightNode->currKeyNum = 0;
        deleteInternal(parent->keys[rightSibling - 1], parent, rightNode);
    }
  }
};

#endif