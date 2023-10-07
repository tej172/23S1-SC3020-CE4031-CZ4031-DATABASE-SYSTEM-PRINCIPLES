

#include "MemoryPool/Disk.cpp"
#include "util/BPlusTree.h"
#include "util/Node.h"
#include "util/Address.h"

#include <random>
#include <string>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Disk d = Disk(20, 400000);

    int key1 = 1;
    int key2 = 1;
    int key3 = 7;
    int key4 = 10;
    int key5 = 17;
    int key6 = 19;
    int key7 = 20;
    int key8 = 21;
    int key9 = 25;
    int key10 = 31;

    Address address1 = d.saveDataToDisk(&key1, sizeof(key1));
    Address address2 = d.saveDataToDisk(&key2, sizeof(key2));
    Address address3 = d.saveDataToDisk(&key3, sizeof(key3));
    Address address4 = d.saveDataToDisk(&key4, sizeof(key4));
    Address address5 = d.saveDataToDisk(&key5, sizeof(key5));
    Address address6 = d.saveDataToDisk(&key6, sizeof(key6));
    Address address7 = d.saveDataToDisk(&key7, sizeof(key7));
    Address address8 = d.saveDataToDisk(&key8, sizeof(key8));
    Address address9 = d.saveDataToDisk(&key9, sizeof(key9));
    Address address10 = d.saveDataToDisk(&key10, sizeof(key10));

    std::cout << "Address1 : blkNumber = " << address1.blkNumber << ", offset = " << address1.offset << std::endl;
    std::cout << "Address2 : blkNumber = " << address2.blkNumber << ", offset = " << address2.offset << std::endl;
    std::cout << "Address3 : blkNumber = " << address3.blkNumber << ", offset = " << address3.offset << std::endl;
    std::cout << "Address4 : blkNumber = " << address4.blkNumber << ", offset = " << address4.offset << std::endl;
    std::cout << "Address5 : blkNumber = " << address5.blkNumber << ", offset = " << address5.offset << std::endl;
    std::cout << "Address6 : blkNumber = " << address6.blkNumber << ", offset = " << address6.offset << std::endl;
    std::cout << "Address7 : blkNumber = " << address7.blkNumber << ", offset = " << address7.offset << std::endl;
    std::cout << "Address8 : blkNumber = " << address8.blkNumber << ", offset = " << address8.offset << std::endl;
    std::cout << "Address9 : blkNumber = " << address9.blkNumber << ", offset = " << address9.offset << std::endl;
    std::cout << "Address10 : blkNumber = " << address10.blkNumber << ", offset = " << address10.offset << std::endl;

    BPlusTree *BPTree = new BPlusTree(3);

    BPTree->insert(address1, 1);
	BPTree->insert(address2, 1);
	BPTree->insert(address3, 7);
	BPTree->insert(address4, 10);
	BPTree->insert(address5, 17);
	BPTree->insert(address6, 19);
	BPTree->insert(address7, 20);
	BPTree->insert(address8, 21);
	BPTree->insert(address9, 25);
	BPTree->insert(address10, 31);

    BPTree->printTree(BPTree->root);

    Address * findAddress1 = BPTree->findSearchKey(1);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(4);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(7);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(10);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(17);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(19);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(20);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(21);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(25);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;
	findAddress1 = BPTree->findSearchKey(31);
	 std::cout << "blkNumber: " << findAddress1->blkNumber << ", offset: " << findAddress1->offset << std::endl;

    

	puts("Hello World!!!");

	return EXIT_SUCCESS;


    return 0;
}
