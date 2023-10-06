#include "../util/Address.h"
#include "../util/Disk.h"
#include <iostream>
#include <cstddef>
#include <cstring>
#include <cmath>

Disk::Disk(int blockSize, int diskSize) {
    this->diskSize = diskSize;
    this->blockSize = blockSize;
    this->availableBlocks = diskSize / blockSize;
    this->currBlockNumber = 0;
    this->startDisk = operator new(diskSize);
    this->block = startDisk;
    this->currBlockMemUsed = 0;
    this->totalBlocks = diskSize / blockSize; // Calculate totalBlocks
}

bool Disk::allocateOneBlock() {
    if (availableBlocks > 0) {
        availableBlocks -= 1;
        currBlockNumber += 1;
        block = (char*)block + blockSize;
        currBlockMemUsed = 0;
        std::cout << "[SUCCESS] Successfully allocated a new block into memory. Memory Used: " << memoryUsed() << " / " << diskSize << '\n';
        return true;
    }
    else {
        std::cout << "[ERROR] Disk doesn't have any memory left to allocate a new block. Memory Used: " << memoryUsed() << " / " << diskSize << '\n';
        return false;
    }
}

Address Disk::writeRecord(std::size_t recordSize) {
    if (recordSize > blockSize) {
        std::cout << "[ERROR] Record Size (" << recordSize << ") is greater than Block Size (" << blockSize << ") " << '\n';
        throw std::runtime_error("An error occurred");
    }

    if (currBlockMemUsed + recordSize > blockSize) {
        bool isSuccess = allocateOneBlock();
        if (!isSuccess) {
            throw std::runtime_error("An error occurred");
        }
    }

    int offset = currBlockMemUsed;
    currBlockMemUsed = currBlockMemUsed + recordSize;

    Address record = { currBlockNumber, offset };
    return record;
}

bool Disk::deleteRecord(Address address, std::size_t sizeToDelete) {
    try {
        void* addressToDelete = (char*)startDisk + address.blkNumber * blockSize + address.offset;
        std::memset(addressToDelete, '\0', sizeToDelete);
        return true;
    }
    catch (...) {
        std::cout << "[ERROR]: Could not remove record block. \n";
        return false;
    }
}

void* Disk::loadDataFromDisk(Address address, std::size_t recordSize) {
    void* data = operator new(recordSize);
    std::memcpy(data, (char*)startDisk + address.blkNumber * blockSize + address.offset, recordSize);
    return data;
}

Address Disk::saveDataToDisk(void* itemAddress, std::size_t recordSize) {
    Address address = writeRecord(recordSize);
    std::memcpy((char*)startDisk + address.blkNumber * blockSize + address.offset, itemAddress, recordSize);
    return address;
}

bool Disk::UpdateDisk(void* itemAddress, std::size_t recordSize, Address address) {
    std::memcpy((char*)startDisk + address.blkNumber * blockSize + address.offset, itemAddress, recordSize);
    return true;
}

int Disk::memoryUsed() {
    ptrdiff_t sizeUsed = (char*)block + currBlockMemUsed - (char*)startDisk;
    return static_cast<int>(sizeUsed);
}

int Disk::getAvailableBlocks() const {
    return availableBlocks;
}


void* Disk::loadDataBlockFromDisk(int blockNumber) const{
    if (blockNumber < 0 || blockNumber >= totalBlocks) {
        std::cerr << "Error: Invalid block number." << std::endl;
        return nullptr;
    }

    return (char*)startDisk + blockNumber * blockSize;
}

int Disk::getNumRecordsInBlock(const void* blockData, std::size_t recordSize) const{
    // Calculate the number of records based on block size and record size
    return blockSize / recordSize;
}

void* Disk::loadRecordFromBlock(const void* blockData, int recordIndex, std::size_t recordSize) const{
    if (recordIndex < 0 || recordIndex >= getNumRecordsInBlock(blockData, recordSize)) {
        std::cerr << "Error: Invalid record index." << std::endl;
        return nullptr;
    }

    return (char*)blockData + recordIndex * recordSize;
}

Address Disk::getNextBlockAddress(const Address& currentAddress) const {
    // Calculate the address of the next block based on the current block's address

    // Check if we're at the last block
    if (currentAddress.blkNumber >= (totalBlocks - 1)) {
        Address nextBlockAddress = { -1, -1 }; // Return -1 to indicate the end
        return nextBlockAddress;
    }

    Address nextBlockAddress = { currentAddress.blkNumber + 1, 0 }; // Go to the next block
    return nextBlockAddress;
}