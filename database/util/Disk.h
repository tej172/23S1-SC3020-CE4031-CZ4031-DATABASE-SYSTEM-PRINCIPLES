#ifndef DISK_H
#define DISK_H

#include "../util/Address.h"
#include <cstddef>

class Disk {
private:
    int diskSize;
    int blockSize;
    int availableBlocks;
    int currBlockMemUsed;
    int currBlockNumber;
    void* startDisk;
    void* block;

public:
    Disk(int blockSize, int diskSize);
    bool allocateOneBlock();
    Address writeRecord(std::size_t recordSize);
    bool deleteRecord(Address address, std::size_t sizeToDelete);
    void* loadDataFromDisk(Address address, std::size_t recordSize);
    Address saveDataToDisk(void* itemAddress, std::size_t recordSize);
    bool UpdateDisk(void* itemAddress, std::size_t recordSize, Address address);
    int memoryUsed();
    int getAvailableBlocks() const;
    int getBlockSize() const {
        return blockSize;
    }

    int getCurrBlockMemUsed() const {
        return currBlockMemUsed;
    }
};

#endif // DISK_H   