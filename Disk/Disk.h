#ifndef DISK_H
#define DISK_H

#include <cstddef>
#include "util/Address.h"
#include <stdexcept>

class Disk {
    public:
        Disk(int blockSize, int diskSize);
        bool deleteRecord(Address address, size_t sizeToDelete);
        void* loadDataFromDisk(Address address, size_t recordSize);
        Address saveDataToDisk(void* itemAddress, size_t recordSize);
        bool UpdateDisk(void* itemAddress, size_t recordSize, Address address);
        int memoryUsed();

    private:
        int diskSize;
        int blockSize;
        int availableBlocks;
        int currBlockNumber;
        void* startDisk;
        void* block;
        int currBlockMemUsed;

        bool allocateOneBlock();
        Address writeRecord(size_t recordSize);
};



#endif // DISK_H
