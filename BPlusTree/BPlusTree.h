#include <vector>
#include "../util/Address.h"

template <typename T>
struct Node {
    bool isLeaf;
    std::size_t maxKey; // maximum number of children
    std::size_t size; // current number of key
    T* key;
    Node<T>** children;
    Node<T>* parent;
    Address * address;

public:
    Node(std::size_t _maxKey) {// Constructor
        this->isLeaf = false;
        this->maxKey = _maxKey;
        this->size = 0;

        T* _key = new T[maxKey-1];
        for(int i=0; i<maxKey-1; i++){
            _key[i] = 0;
        }
        this->key = _key;
        

        Node<T>** _children = new Node<T>*[maxKey];
        for(int i=0; i<maxKey; i++){
            _children[i] = nullptr;
        }
        this->children = _children;

        address = new Address[maxKey];
        this->parent = nullptr;
    }
};

template <typename T>
class BPlusTree {
    Node<T>* root;
    std::size_t maxKey;

public:
    BPlusTree(std::size_t _maxKey) {// Constructor
        this->root = nullptr;
        this->maxKey = _maxKey;
    }
    ~BPlusTree() { // Destructor
        clear(this->root);
    }

    Node<T>* getroot(){
        return this->root;
    }

    Node<T>* BPlusTreeRangeSearch(Node<T>* node, T key){
        if(node == nullptr) { // if root is null, return nullptr
            return nullptr;
        }
        else{
            Node<T>* cursor = node; // cursor finding key

            while(!cursor->isLeaf){ // until cusor pointer arrive leaf
                for(int i=0; i<cursor->size; i++){ //in this index node, find what we want key
                    if(key < cursor->key[i]){ //find some range, and let find their child also.
                        cursor = cursor->children[i];
                        break;
                    }
                    if(i == (cursor->size)-1){
                        cursor = cursor->children[i+1];
                        break;
                    }
                }
            }
            return cursor;
        }
    }

    int findIndex(T* arr, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < arr[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }
        return index;
    }
    int keyInsertIndex(T* arr, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < arr[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }

        return index;
    }
    T* keyInsert(T* arr, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < arr[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }

        for(int i = len; i > index; i--){
            arr[i] = arr[i-1];
        }

        arr[index] = data;

        return arr;
    }
    int getIndex(T* key_array, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < key_array[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }

        return index;
    }
    Node<T>** childrenInsert(Node<T>** child_arr, Node<T>*child,int len,int index){
        for(int i= len; i > index; i--){
            child_arr[i] = child_arr[i - 1];

        }
        child_arr[index] = child;
        return child_arr;
    }
    Node<T>* childrenKeyInsert(Node<T>* node, T data, Node<T>* child){
        int key_index=0;
        int child_index=0;
        for(int i=0; i< node->size; i++){
            if(data < node->key[i]){
                key_index = i;
                child_index = i+1;
                break;
            }
            if(i==node->size-1){
                key_index = node->size;
                child_index = node->size+1;
                break;
            }
        }
        for(int i = node->size;i > key_index; i--){
            node->key[i] = node->key[i-1];
        }
        for(int i=node->size+1;i>child_index;i--){
            node->children[i] = node->children[i-1];
        }

        node->key[key_index] = data;
        node->children[child_index] = child;

        return node;
    }
    void parentInsert(Node<T>* par,Node<T>* child, T data){
        //overflow check
        Node<T>* cursor = par;
        if(cursor->size < this->maxKey-1){//not overflow, just insert in the correct position
            //insert key, child, and reallocate
            cursor = childrenKeyInsert(cursor,data,child);
            cursor->size++;
        }
        else{//overflow
            //make new node
            auto* Newnode = new Node<T>(this->maxKey);
            Newnode->parent = cursor->parent;

            //copy key
            T* key_copy = new T[cursor->size+1];

            for(int i=0; i<cursor->size; i++){
                key_copy[i] = cursor->key[i];

            }
            key_copy = keyInsert(key_copy,data,cursor->size);


            auto** child_copy = new Node<T>*[cursor->size+2];
            for(int i=0; i<cursor->size+1;i++){
                child_copy[i] = cursor->children[i];
            }
            child_copy[cursor->size+1] = nullptr;
            child_copy = childrenInsert(child_copy,child,cursor->size+1,findIndex(key_copy,data,cursor->size+1));

            //split nodes
            cursor->size = (this->maxKey)/2;
            if((this->maxKey) % 2 == 0){
                Newnode->size = (this->maxKey) / 2 -1;
            }
            else{
                Newnode->size = (this->maxKey) / 2;
            }

            for(int i=0; i<cursor->size;i++){
                cursor->key[i] = key_copy[i];
                cursor->children[i] = child_copy[i];
            }
            cursor->children[cursor->size] = child_copy[cursor->size];

            for(int i=0; i < Newnode->size; i++){
                Newnode->key[i] = key_copy[cursor->size + i +1];
                Newnode->children[i] = child_copy[cursor->size+i+1];
                Newnode->children[i]->parent=Newnode;
            }
            Newnode->children[Newnode->size] = child_copy[cursor->size+Newnode->size+1];
            Newnode->children[Newnode->size]->parent=Newnode;

            T parkey = key_copy[this->maxKey/2];

            delete[] key_copy;
            delete[] child_copy;

            //parent check
            if(cursor->parent == nullptr){//if there are no parent node(root case)
                auto* Newparent = new Node<T>(this->maxKey);
                cursor->parent = Newparent;
                Newnode->parent = Newparent;

                Newparent->key[0] = parkey;
                Newparent->size++;

                Newparent->children[0] = cursor;
                Newparent->children[1] = Newnode;

                this->root = Newparent;

                //delete Newparent;
            }
            else{//if there already have parent node
                parentInsert(cursor->parent, Newnode, parkey);
            }
        }
    }
    void insert(T data, Address address) {
        if(this->root == nullptr){ //if the tree is empty
            this->root = new Node<T>(this->maxKey);
            this->root->isLeaf = true;
            this->root->key[0] = data;
            this->root->address[0] = address;
            this->root->size = 1; //
        }
        else{ //if the tree has at least one node
            Node<T>* cursor = this->root;

            //move to leaf node
            cursor = BPlusTreeRangeSearch(cursor, data);

            //overflow check
            if(cursor->size < (this->maxKey-1)){ // not overflow, just insert in the correct position
                //key insert and 
                int index = getIndex(cursor->key,data,cursor->size);

                for(int i = cursor->size; i > index; i--){
                    cursor->key[i] = cursor->key[i-1];
                    cursor->address[i] = cursor->address[i-1];
                }

                cursor->key[index] = data;
                cursor->address[index] = address;

                cursor->size++;
                //edit pointer(next node)
                cursor->children[cursor->size] = cursor->children[cursor->size-1];
                cursor->children[cursor->size-1] = nullptr;
            }
            else{//overflow case
                //make new node
                auto* Newnode = new Node<T>(this->maxKey);
                Newnode->isLeaf = true;
                Newnode->parent = cursor->parent;

                //copy key
                T* key_copy = new T[cursor->size+1];
                Address * address_copy = new Address[cursor->size+1];
                for(int i=0; i<cursor->size; i++){
                    key_copy[i] = cursor->key[i];
                    address_copy[i] = cursor->address[i];
                }

                //insert and rearrange
                int index = getIndex(key_copy,data,cursor->size);

                for(int i = cursor->size; i > index; i--){
                    key_copy[i] = key_copy[i-1];
                    address_copy[i] = address_copy[i-1];
                }

                key_copy[index] = data;
                address_copy[index] = address;

                //split nodes
                cursor->size = (this->maxKey)/2;
                if((this->maxKey) % 2 == 0){
                    Newnode->size = (this->maxKey) / 2;
                }
                else{
                    Newnode->size = (this->maxKey) / 2 + 1;
                }

                for(int i=0; i<cursor->size;i++){
                    cursor->key[i] = key_copy[i];
                    cursor->address[i] = address_copy[i];
                }
                for(int i=0; i < Newnode->size; i++){
                    Newnode->key[i] = key_copy[cursor->size + i];
                    Newnode->address[i] = address_copy[cursor->size + i];
                }

                cursor->children[cursor->size] = Newnode;
                Newnode->children[Newnode->size] = cursor->children[this->maxKey-1];
                cursor->children[this->maxKey-1] = nullptr;

                delete[] key_copy;
                delete[] address_copy;
                
                //parent check
                T parkey = Newnode->key[0];

                if(cursor->parent == nullptr){//if there are no parent node(root case)
                    auto* Newparent = new Node<T>(this->maxKey);
                    cursor->parent = Newparent;
                    Newnode->parent = Newparent;

                    Newparent->key[0] = parkey;
                    Newparent->size++;

                    Newparent->children[0] = cursor;
                    Newparent->children[1] = Newnode;

                    this->root = Newparent;
                }
                else{//if there already have parent node
                    parentInsert(cursor->parent, Newnode, parkey);
                }
            }
        }
    }

    void remove(T data) { // Remove an key from the tree.
        //make cursor
        Node<T>* cursor = this->root;

        //move to leaf node
        cursor = BPlusTreeRangeSearch(cursor,data);

        //make sibling index
        int sib_index =-1;
        for(int i=0; i<cursor->parent->size+1;i++){
            if(cursor == cursor->parent->children[i]){
                sib_index = i;
            }
        }
        int left=sib_index-1;
        int right=sib_index+1;


        //find data
        int del_index=-1;
        for(int i=0; i< cursor->size; i++){
            if(cursor->key[i] == data){
                del_index = i;
                break;
            }
        }
        //if data dosen't exist, nothing happen
        if(del_index==-1){
            return; // there is no match remove value
        }

        //remove data
        for(int i=del_index; i<cursor->size-1;i++){
            cursor->key[i] = cursor->key[i+1];
            cursor->address[i] = cursor->address[i+1];
        }
        cursor->key[cursor->size-1] = 0;
        cursor->address[cursor->size-1] = Address();
        cursor->size--;

        //if cursor is root, and there are no more data -> clean!
        if(cursor == this->root && cursor->size==0){//root case
            clear(this->root);
            this->root = nullptr;
            return;
        }
        cursor->children[cursor->size] = cursor->children[cursor->size+1];
        cursor->children[cursor->size+1] = nullptr;


        //underflow check
        if(cursor == this->root){
            return;
        }
        if(cursor->size < maxKey/2){//underflow case

            if(left >= 0){// left_sibiling exists
                Node<T>* leftsibling= cursor->parent->children[left];

                if(leftsibling->size > maxKey/2){ //if data number is enough to use this node
                    T* temp = new T[cursor->size+1];
                    Address * temp_address = new Address[cursor->size+1];

                    //copy key
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->key[i];
                        temp_address[i] = cursor->address[i];
                    }

                    //insert and rearrange
                    int index = keyInsertIndex(temp,leftsibling->key[leftsibling->size -1],cursor->size);
                    
                    for(int i = cursor->size; i > index; i--){
                        temp[i] = temp[i-1];
                        temp_address[i] = temp_address[i-1];
                    }

                    temp[index] = leftsibling->key[leftsibling->size -1];
                    temp_address[index] =leftsibling->address[leftsibling->size -1];
                    
                    
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->key[i] = temp[i];
                        cursor->address[i] = temp_address[i];
                    }
                    cursor->size++;
                    delete[] temp;

                    //pointer edit
                    cursor->children[cursor->size] = cursor->children[cursor->size-1];
                    cursor->children[cursor->size-1] = nullptr;

                    //sibling property edit
                    leftsibling->key[leftsibling->size-1] = 0;
                    leftsibling->address[leftsibling->size-1] = Address();
                    leftsibling->size--;
                    leftsibling->children[leftsibling->size] = leftsibling->children[leftsibling->size+1]; //cursor
                    leftsibling->children[leftsibling->size+1]= nullptr;

                    //parent property edit
                    cursor->parent->key[left] = cursor->key[0];
                    return;
                }
            }
            if(right <= cursor->parent->size){// right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                if(rightsibling->size >maxKey/2){//if data number is enough to use this node
                    T* temp = new T[cursor->size+1];
                    Address * temp_address = new Address[cursor->size+1];

                    //copy key
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->key[i];
                        temp_address[i] = cursor->address[i];
                    }
                    //insert and rearrange
                    int index = keyInsertIndex(temp,rightsibling->key[0],cursor->size);
                    for(int i = cursor->size; i > index; i--){
                        temp[i] = temp[i-1];
                        temp_address[i] = temp_address[i-1];
                    }

                    temp[index] = rightsibling->key[0];
                    temp_address[index] = rightsibling->address[0];


                    for(int i=0; i<cursor->size+1; i++){
                        cursor->key[i] = temp[i];
                        cursor->address[i] = temp_address[i];
                    }
                    cursor->size++;
                    delete[] temp;
                    delete [] temp_address;

                    //pointer edit
                    cursor->children[cursor->size] = cursor->children[cursor->size-1];
                    cursor->children[cursor->size-1] = nullptr;

                    //sibling property edit
                    for(int i=0; i<rightsibling->size-1;i++){
                        rightsibling->key[i] = rightsibling->key[i+1];
                        rightsibling->address[i] = rightsibling->address[i+1];
                    }
                    rightsibling->key[rightsibling->size-1] = 0;
                    rightsibling->address[rightsibling->size-1] = Address();
                    rightsibling->size--;
                    rightsibling->children[rightsibling->size] = rightsibling->children[rightsibling->size+1]; //cursor
                    rightsibling->children[rightsibling->size+1]= nullptr;

                    //parent property edit
                    cursor->parent->key[right-1] = rightsibling->key[0];

                    return;
                }
            }

            //if sibling is not enought to use their data
            //we have to merge step

            if(left>=0){ // left_sibling exists
                Node<T>* leftsibling = cursor->parent->children[left];

                //merge two leaf node
                for(int i=0; i<cursor->size; i++){
                    leftsibling->key[leftsibling->size+i]=cursor->key[i];
                    leftsibling->address[leftsibling->size+i]=cursor->address[i];
                }
                //edit pointer
                leftsibling->children[leftsibling->size] = nullptr;
                leftsibling->size = leftsibling->size+cursor->size;
                leftsibling->children[leftsibling->size] = cursor->children[cursor->size];

                //parent property edit
                removeParent(cursor, left, cursor->parent);
                for(int i=0; i<cursor->size;i++){
                    cursor->key[i]=0;
                    cursor->address[i] = Address();
                    cursor->children[i] = nullptr;
                }
                cursor->children[cursor->size] = nullptr;

                delete[] cursor->key;
                delete[] cursor->children;
                delete[] cursor->address;
                delete cursor;

                return;

            }
            if(right<=cursor->parent->size){ // right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                //merge two leaf node
                for(int i=0; i<rightsibling->size; i++){
                    cursor->key[i+cursor->size]=rightsibling->key[i];
                    cursor->address[i+cursor->size]=rightsibling->address[i];
                }
                //edit pointer
                cursor->children[cursor->size] = nullptr;
                cursor->size = rightsibling->size+cursor->size;
                cursor->children[cursor->size] = rightsibling->children[rightsibling->size];

                //parent property edit
                removeParent(rightsibling, right-1, cursor->parent);

                for(int i=0; i<rightsibling->size;i++){
                    rightsibling->key[i]=0;
                    rightsibling->address[i] = Address();
                    rightsibling->children[i] = nullptr;
                }
                rightsibling->children[rightsibling->size] = nullptr;

                delete[] rightsibling->key;
                delete[] rightsibling->children;
                delete[] rightsibling->address;
                delete rightsibling;
                return;

            }

        }
        else{
            return;
        }
    }
    void removeParent(Node<T>* node, int index, Node<T>* par){
        Node<T>* remover = node;
        Node<T>* cursor = par;
        T target = cursor->key[index];

        //if cursor is root, and there are no more data -> child node is to be root!
        if(cursor == this->root && cursor->size==1){//root case
            if(remover == cursor->children[0]){
                delete[] remover->key;
                delete[] remover->children;
                delete remover;
                this->root = cursor->children[1];
                delete[] cursor->key;
                delete[] cursor->children;
                delete cursor;
                return;
            }
            if(remover == cursor->children[1]){
                delete[] remover->key;
                delete[] remover->children;
                delete remover;
                this->root = cursor->children[0];
                delete[] cursor->key;
                delete[] cursor->children;
                delete cursor;
                return;
            }
        }

        //remove data
        for(int i=index; i<cursor->size-1;i++){
            cursor->key[i] = cursor->key[i+1];
        }
        cursor->key[cursor->size-1] = 0;

        //remove pointer
        int rem_index = -1;
        for(int i=0; i<cursor->size+1;i++){
            if(cursor->children[i] == remover){
                rem_index = i;
            }
        }
        if(rem_index == -1){
            return;
        }
        for(int i=rem_index; i<cursor->size;i++){
            cursor->children[i] = cursor->children[i+1];
        }
        cursor->children[cursor->size] = nullptr;
        cursor->size--;

        //underflow check
        if(cursor == this->root){
            return;
        }
        if(cursor->size < maxKey/2){//underflow case

            int sib_index =-1;
            for(int i=0; i<cursor->parent->size+1;i++){
                if(cursor == cursor->parent->children[i]){
                    sib_index = i;
                }
            }
            int left=sib_index-1;
            int right=sib_index+1;

            if(left >= 0){// left_sibiling exists
                Node<T>* leftsibling= cursor->parent->children[left];

                if(leftsibling->size > maxKey/2){ //if data number is enough to use this node
                    T* temp = new T[cursor->size+1];

                    //copy key
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->key[i];
                    }

                    //insert and rearrange at cursor
                    keyInsert(temp, cursor->parent->key[left],cursor->size);
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->key[i] = temp[i];
                    }
                    cursor->parent->key[left] = leftsibling->key[leftsibling->size-1];
                    delete[] temp;

                    Node<T>** child_temp = new Node<T>*[cursor->size+2];
                    //copy child node
                    for(int i=0; i<cursor->size+1; i++){
                        child_temp[i]=cursor->children[i];
                    }
                    //insert and rearrange at child
                    childrenInsert(child_temp,leftsibling->children[leftsibling->size],cursor->size,0);

                    for(int i=0; i<cursor->size+2; i++){
                        cursor->children[i] = child_temp[i];
                    }
                    delete[] child_temp;

                    //size edit
                    cursor->size++;
                    leftsibling->size--;
                    return;

                }
            }

            if(right <= cursor->parent->size){// right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                if(rightsibling->size > maxKey/2){//if data number is enough to use this node
                    T* temp = new T[cursor->size+1];

                    //copy key
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->key[i];
                    }
                    //insert and rearrange at cursor
                    keyInsert(temp,cursor->parent->key[sib_index],cursor->size);
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->key[i] = temp[i];
                    }
                    cursor->parent->key[sib_index] = rightsibling->key[0];
                    delete[] temp;

                    //insert and reaarange at child

                    cursor->children[cursor->size+1] = rightsibling->children[0];
                    for(int i=0; i<rightsibling->size; i++){
                        rightsibling->children[i] = rightsibling->children[i+1];
                    }
                    rightsibling->children[rightsibling->size] = nullptr;

                    cursor->size++;
                    rightsibling->size--;
                    return;

                }
            }

            //if sibling is not enought to use their data
            //we have to merge step
            if(left>=0){ // left_sibling exists
                Node<T>* leftsibling = cursor->parent->children[left];

                leftsibling->key[leftsibling->size] = cursor->parent->key[left];
                //merge two leaf node
                for(int i=0; i<cursor->size; i++){
                    leftsibling->key[leftsibling->size+i+1]=cursor->key[i];
                }
                for(int i=0; i<cursor->size+1;i++){
                    leftsibling->children[leftsibling->size+i+1] = cursor->children[i];
                    cursor->children[i]->parent = leftsibling;
                }
                for(int i=0; i<cursor->size+1; i++){
                    cursor->children[i] = nullptr;
                }
                leftsibling->size = leftsibling->size+cursor->size+1;
                //delete recursion
                removeParent(cursor, left,cursor->parent);
                return;

            }
            if(right<=cursor->parent->size){ // right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                cursor->key[cursor->size] = cursor->parent->key[right-1];
                //merge two leaf node
                for(int i=0; i<rightsibling->size; i++){
                    cursor->key[cursor->size+1+i]=rightsibling->key[i];
                }
                for(int i=0; i<rightsibling->size+1;i++){
                    cursor->children[cursor->size+i+1] = rightsibling->children[i];
                    rightsibling->children[i]->parent=rightsibling;
                }
                for(int i=0; i<rightsibling->size+1; i++){
                    rightsibling->children[i] = nullptr;
                }
                //edit pointer
                rightsibling->size = rightsibling->size+cursor->size+1;
                //parent property edit
                removeParent(rightsibling, right-1,cursor->parent);
                return;
            }
        }
        else{
            return;
        }
    }

    void clear(Node<T>* cursor){
        if(cursor != nullptr){
            if(!cursor->isLeaf){
                for(int i=0; i <= cursor->size; i++){
                    clear(cursor->children[i]);
                }
            }
            delete[] cursor->key;
            delete[] cursor->children;
            delete[] cursor->address;
            delete cursor;
        }
    }
    void bpt_print(){
        print(this->root);
    }
    void print(Node<T>* cursor) {
        // You must NOT edit this function.
        if (cursor != NULL) {
            for (int i = 0; i < cursor->size; ++i) {
                std::cout << cursor->key[i] << " ";
            }
            std::cout << "\n";

            if (!cursor->isLeaf) {
                for (int i = 0; i < cursor->size + 1; ++i) {
                    print(cursor->children[i]);
                }
            }
        }
    }

    Node<float> * findFirstMostNode()
    {
        Node<float> *currNode = getroot();

        // std::cout << currNode->item[0];

        while (currNode->isLeaf == false)
        {
            currNode = currNode->children[0];
        }

        std::cout << "\nLEFTMOST VALUE::  " << currNode->key[0] << "\n";
        //return currNode->item[0];
        return currNode;

    }
    // void delValues(std::vector<float> floatVector)
    // {
    //     for (int i = 0; i < floatVector.size(); i++)
    //     {
    //         remove(floatVector[i]);
    //     }
    // }

        int delKeyRange(float startKey, float endKey)
    {

        std::vector<Address> delValues;

        delValues = findKeyRange(startKey, endKey);

        Node<float> *currNode = findCorrectNodeForKey(startKey, getroot());
        bool keepDeleting = true;
        std::vector<float> floatVector;
        int delCount=0;

        std::cout << "\nCOUNT 1 IS: " << delCount << "\n";

        //for (int i=0; i< delValues.size(); i++){
            //remove(delValues[i]);
            //std::cout << "VALUE IS: " << delValues.size();
        
        if (currNode != nullptr)
        {
            for (int j = 0; j < currNode->size; j++)
            {
                if (currNode->key[j] >= startKey && currNode->key[j] <= endKey)
                {
                    //std::cout << "\nkey " << j << "  is " << currNode->key[j];
                    //remove(currNode->key[j]);
                    floatVector.push_back(currNode->key[j]);
                    delCount++;
                    keepDeleting = true;
                }
                else
                {
                    keepDeleting = false;
                }
            }
        }
        std::cout << "\nCOUNT 2 IS: " << delCount << "\n";

        while (keepDeleting == true)
        {
            if (currNode->children[currNode->size] != nullptr){
                currNode = currNode->children[currNode->size];
                for (int i = 0; i < currNode->size; i++)
                {
                    if (currNode->key[i] >= startKey && currNode->key[i] <= endKey)
                    {
                        //std::cout << "\nkey "<<i<< "  is " << currNode->key[i];
                        floatVector.push_back(currNode->key[i]);

                        //remove(currNode->key[i]);

                        delCount++;
                    }
                    else
                    {
                        keepDeleting = false;
                        std::cout<<"no work";

                        for (int m = 0; m < floatVector.size(); m++)
                        {
                            //std::cout << "m:" << floatVector[m] << " ";
                            remove(floatVector[m]);
                            // remove(0.25);
                            // remove(0.257);
                            // remove(0.266);
                            // remove(0.269);
                            // remove(0.274);
                            // remove(0.275);
                            // remove(0.277);
                        }
                        return delCount;
                    }
                }
            }
            /* code */
            else{
                keepDeleting = false;
            }
        }
        
        //}

        std::cout << "\nkeepDeleting(BOOL) IS:: "<< keepDeleting << "\n";
        std::cout << "FINAL COUNT IS: " << delCount << "\n";

        for (int i = 0; i < floatVector.size(); i++)
        {
            //std::cout << "i:" << floatVector[i] << " ";

            remove(floatVector[i]);
        }
        return delCount;
    }

    Node<float> *findCorrectNodeForKey(float searchKey, Node<T> *rootPtr = nullptr)
    {
        Node<float> * searchPtr;

        //std::cout << "[STARTING SEARCH] searchKey: " << searchKey << " \n";
        searchPtr = rootPtr;

        //size = currKeyNum
        while (searchPtr->isLeaf == false){
            //traverse through each visited node
            for (int i=0; i< searchPtr->size; i++){
                //std::cout << "[SEARCHING INTERNAL NODE] Node Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                if (searchKey < searchPtr->key[i]){
                    //std::cout << "[INTERNAL NODE FOUND] < searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    searchPtr = searchPtr->children[i];
                    break;
                }

                if (searchKey == searchPtr->key[i]){
                    //std::cout << "[INTERNAL NODE FOUND] = searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    //need to search for left and right node 
                    Node<float> * leftNode = searchPtr->children[i];
                    Node<float> * rightNode = searchPtr->children[i+1];

                    leftNode = findCorrectNodeForKey(searchKey, leftNode);
                    rightNode = findCorrectNodeForKey(searchKey, rightNode);

                    searchPtr = (leftNode == nullptr && rightNode == nullptr) ? nullptr :
                                (leftNode != nullptr && rightNode == nullptr) ? leftNode :
                                (leftNode == nullptr && rightNode != nullptr) ? rightNode :
                                leftNode;
                    break;
                }

                if (i == searchPtr->size - 1){
                    //std::cout << "[NODE FOUND] >= searchKey. Key: " << searchPtr->keys[i] << ", searchKey: " << searchKey << " \n";
                    searchPtr = searchPtr->children[i+1];
                    break;
                }
            }  
        }

        return searchPtr;
    };

  std::vector<Address> findKeyRange(float startKey, float endKey){
    Node<float> * currNode = findCorrectNodeForKey(startKey, getroot());
    bool keepSearching = true;
    std::vector<Address> vec;
    int count = 0;

    if(currNode != nullptr){
        for (int i=0; i< currNode->size; i++){
            if (currNode->key[i] >= startKey && currNode->key[i] <= endKey){
                vec.push_back(currNode->address[i]); 
                count ++; 
                keepSearching = true;
            }else{
                keepSearching = false;
            }
        }
    }

    while (keepSearching){
        if ( currNode->children[currNode->size] != nullptr){
            currNode = currNode->children[currNode->size];
            for (int i=0; i< currNode->size; i++){
                if (currNode->key[i] >= startKey && currNode->key[i] <= endKey){
                    vec.push_back(currNode->address[i]);
                    count ++; 
                }else{
                    keepSearching = false;
                    return vec;
                }
            }
        }else{
            keepSearching= false;
        }
    }

    // // Print the contents of vec
    // for (const Address &address : vec)
    // {
    //     std::cout << address << std::endl;
    // }

    return vec;
  }

};