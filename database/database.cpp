//============================================================================
// Name        : database.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include "MemoryPool/MemoryPool.cpp"
#include "util/BPlusTree.h"
#include "util/Node.h"
#include "util/Address.h"

#include <random>
#include <string>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	// testing the memory
	// std::string data1 = "hello";
	// std::string data2 = "testingtesting";
	// std::string data3 = "ehhhhhhhhhhhh";

	// Disk d(400, 300000000);

	// Address add1 = d.saveDataToDisk(&data1, sizeof(std::string)*16);
	// Address add2 = d.saveDataToDisk(&data2, sizeof(std::string));
	// Address add3 = d.saveDataToDisk(&data3, sizeof(std::string));

	// std::cout << "Block Number: " << add1.blkNumber << std::endl;
	// std::cout << "Offset: " << add1.offset << std::endl;

	// std::cout << "Block Number: " << add2.blkNumber << std::endl;
	// std::cout << "Offset: " << add2.offset << std::endl;

	// std::cout << "Block Number: " << add3.blkNumber << std::endl;
	// std::cout << "Offset: " << add3.offset << std::endl;

	// void * fromDiskdata1 = d.loadDataFromDisk(add1, sizeof(std::string) );
	// void * fromDiskdata2 = d.loadDataFromDisk(add2, sizeof(std::string) );
	// void * fromDiskdata3 = d.loadDataFromDisk(add3, sizeof(std::string) );
	
	// std::string* strPtr = static_cast<std::string*>(fromDiskdata1);
	// std::cout << *strPtr << std::endl;

	// std::string* strPtr2 = static_cast<std::string*>(fromDiskdata2);
	// std::cout << *strPtr2 << std::endl;

	// std::string* strPtr3 = static_cast<std::string*>(fromDiskdata3);
	// std::cout << *strPtr3 << std::endl;


	//testing the search B+ Tree
	// BPlusTree *BPTree = new BPlusTree(3);
	
	// Node *root = new Node(3);
	
	// Node *lvl1_1 = new Node(3);

	// Node *lvl2_1 = new Node(3);

	// lvl2_1->isLeaf = true;
	// lvl2_1->keys[0] = 2;
	// lvl2_1->keys[1] = 3;
	// lvl2_1->keys[2] = 5;
	// lvl2_1->currKeyNum = 3;

	// Node *lvl2_2 = new Node(3);
	// lvl2_2->isLeaf = true;
	// lvl2_2->keys[0] = 7;
	// lvl2_2->keys[1] = 13;
	// lvl2_2->currKeyNum = 2;

	// lvl1_1->isLeaf = false;
	// lvl1_1->nodePtrs[0] = lvl2_1;
	// lvl1_1->nodePtrs[1] = lvl2_2;
	// lvl1_1->keys[0] = 7;
	// lvl1_1->currKeyNum = 1;

	// Node *lvl1_2 = new Node(3);

	// Node *lvl2_3  = new Node(3);
	// lvl2_3->isLeaf = true;
	// lvl2_3->keys[0] = 13;
	// lvl2_3->keys[1] = 23;
	// lvl2_3->keys[2] = 23;
	// lvl2_3->currKeyNum = 3;

	// Node *lvl2_4  = new Node(3);
	// lvl2_4->isLeaf = true;
	// lvl2_4->keys[0] = 25;
	// lvl2_4->keys[1] = 25;
	// lvl2_4->currKeyNum = 2;

	// Node *lvl2_6  = new Node(3);
	// lvl2_6->isLeaf = true;
	// lvl2_6->keys[0] = 43;
	// lvl2_6->keys[1] = 47;
	// lvl2_6->currKeyNum = 2;

	// Node *lvl2_5  = new Node(3);
	// lvl2_5->isLeaf = true;
	// lvl2_5->keys[0] = 26;
	// lvl2_5->keys[1] = 37;
	// lvl2_5->keys[2] = 41;
	// lvl2_5->currKeyNum = 3;

	// lvl1_2->isLeaf = false;
	// lvl1_2->nodePtrs[0] = lvl2_3;
	// lvl1_2->nodePtrs[1] = lvl2_4;
	// lvl1_2->nodePtrs[2] = lvl2_5;
	// lvl1_2->nodePtrs[3] = lvl2_6;
	// lvl1_2->keys[0] = 25;
	// lvl1_2->keys[1] = 26;
	// lvl1_2->keys[2] = 43;
	// lvl1_2->currKeyNum = 3;

	// root->isLeaf = false;
	// root->nodePtrs[0] = lvl1_1;
	// root->nodePtrs[1] = lvl1_2;
	// root->keys[0] = 13;
	// root->currKeyNum = 1;
	
	// BPTree->root = root;
	// BPTree->findSearchKey(23);	
	// BPTree->print();

	//Testing inserting B+ Tree
	BPlusTree *BPTree = new BPlusTree(3);
	Address * address = new Address();

	
	// BPTree->insert(*address, 2);
	// BPTree->insert(*address, 3);
	// BPTree->insert(*address, 5);
	// BPTree->insert(*address, 7);

	// BPTree->insert(*address, 13);
	// BPTree->insert(*address, 13);
	// BPTree->insert(*address, 23);
	// BPTree->insert(*address, 23);
	// BPTree->insert(*address, 25);
	// BPTree->insert(*address, 25);
	// BPTree->insert(*address, 26);
	// BPTree->insert(*address, 37);
	// BPTree->insert(*address, 41);
	// BPTree->insert(*address, 43);
	// BPTree->insert(*address, 47);	

	// // BPTree->print(BPTree->root);

	// for (int i = 13; i < 38; i++){
	// 	std::cout << i << "\n ";
	// 	BPTree->findSearchKey(i);	
	// }

	//test for teg delete

	BPTree->deleteNode(1);

	puts("Hello World!!!");

	return EXIT_SUCCESS;
}
