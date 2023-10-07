#ifndef DISK_H
#define DISK_H

#include <cstddef>
#include "../util/Address.h"
#include <stdexcept>

class Disk {
    public:
        Disk(int blockSize, int diskSize){
            this->diskSize = diskSize;
            this->blockSize = blockSize;
            this->availableBlocks = diskSize / blockSize;
            this->currBlockNumber = 0;
            this->startDisk = operator new(diskSize);
            this->block = startDisk;
            this->currBlockMemUsed = 0;
        }
        bool deleteRecord(Address address, size_t sizeToDelete) {
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

        void* loadDataFromDisk(Address address, size_t recordSize) {
            void* data = operator new(recordSize);
            std::memcpy(data, (char*)startDisk + address.blkNumber * blockSize + address.offset, recordSize);
            return data;
        }

        Address saveDataToDisk(void* itemAddress, size_t recordSize) {
            Address address = writeRecord(recordSize);
            std::memcpy((char*)startDisk + address.blkNumber * blockSize + address.offset, itemAddress, recordSize);
            return address;
        }

        bool UpdateDisk(void* itemAddress, size_t recordSize, Address address) {
            std::memcpy((char*)startDisk + address.blkNumber * blockSize + address.offset, itemAddress, recordSize);
            return true;
        }

        int memoryUsed() {
            ptrdiff_t sizeUsed = (char*)block + currBlockMemUsed - (char*)startDisk;
            return static_cast<int>(sizeUsed);
        }


    private:
        int diskSize;
        int blockSize;
        int availableBlocks;
        int currBlockNumber;
        void* startDisk;
        void* block;
        int currBlockMemUsed;

        bool allocateOneBlock() {
            if (availableBlocks > 0) {
                availableBlocks -= 1;
                currBlockNumber += 1;
                block = (char*)block + blockSize;
                currBlockMemUsed = 0;
                //std::cout << "[SUCCESS] Successfully allocated a new block into memory. Memory Used: " << memoryUsed() << " / " << diskSize << '\n';
                return true;
            }
            else {
                std::cout << "[ERROR] Disk doesn't have any memory left to allocate a new block. Memory Used: " << memoryUsed() << " / " << diskSize << '\n';
                return false;
            }
        }

        Address writeRecord(std::size_t recordSize) {
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
};



#endif // DISK_H
