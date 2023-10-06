#ifndef DISK_H
#define DISK_H

#include <cstddef>
#include "util/Address.h"
#include <stdexcept>

class Disk {
private:
    int diskSize;
    int blockSize;
    int availableBlocks;
    int currBlockMemUsed;
    int currBlockNumber;
    void* startDisk;
    void* block;
    int totalBlocks;

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
    void* loadDataBlockFromDisk(int blockNumber) const;
    int getNumRecordsInBlock(const void* blockData, std::size_t recordSize) const;
    void* loadRecordFromBlock(const void* blockData, int recordIndex, std::size_t recordSize) const;
    int getBlockSize() const {
        return blockSize;
    }

    int getCurrBlockMemUsed() const {
        return currBlockMemUsed;
    }

     int getNumBlocks() const {
        return totalBlocks;
    }

     Address getNextBlockAddress(const Address& currentAddress) const;
      // Destructor
//   ~Disk();
};

#endif // DISK_H   