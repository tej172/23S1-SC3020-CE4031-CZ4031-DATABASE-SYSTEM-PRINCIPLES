#include "../util/Address.h"

#include <iostream>
#include <cstddef>
#include <cstring>
#include <cmath>

class Disk{
    int diskSize; 
    int blockSize;
    int availableBlocks;  //keeps track of num of Available block, will decrement by one everytime block is allocated
    
    int currBlockMemUsed; //keeps track of how many memory is already is used in the curr block being pointed 

    void *startDisk; // pointer pointing to the start of disk 
    void *block; //pointer pointing to the end of disk 


    public:
        Disk(int blockSize, int diskSize){
            this->diskSize = diskSize; 
            this->blockSize = blockSize;
            this->availableBlocks = diskSize / blockSize;
            
            this->startDisk = operator new(diskSize); //allocate diskSize amount of memory in the heap, and make it point ot the start of this memory
            this->block = startDisk; //initially start at the startDisk 

            this->currBlockMemUsed = 0; 

        };

        bool allocateOneBlock(){
            if (availableBlocks > 0){
                availableBlocks -= 1; //decrement the num of available blocks by 1
                //typecasted to char * so it moves 1 byte away from blockSize, if typecasted to another type -> e.g. int, then +1 will move it 4 bytes away 
                block = (char *)block + blockSize;
                currBlockMemUsed = 0;
                std::cout << "[SUCCESS] Successfully allocate a new block into memory. Memory Used: " << memoryUsed() << " / " << diskSize << '\n'; 
                return true;
            }else{
                std::cout << "[ERROR] Disk doesn't have any memory left to allocate a new block. Memory Used: " << memoryUsed() << " / " << diskSize << '\n'; 
                return false;
            }

        }

        Address writeRecord(std::size_t recordSize){
            if(recordSize > blockSize){
                std::cout << "[ERROR] Record Size (" << recordSize << ") is greater than Block Size (" << blockSize << ") " << '\n'; 
                throw std::runtime_error("An error occurred");
            }

            if(currBlockMemUsed + recordSize > blockSize){
                bool isSuccess = allocateOneBlock();
                if(!isSuccess){
                    throw std::runtime_error("An error occurred");
                }
            }
            
            int offset = currBlockMemUsed;
            currBlockMemUsed = currBlockMemUsed + recordSize;    

            Address record = {currBlockNumber(), offset};
            return record;
        };        

        bool deleteRecord(Address address, std::size_t sizeToDelete){
            try{
                // Remove record from block.
                void *addressToDelete = (char*)startDisk + address.blkNumber * blockSize + address.offset;
                std::memset(addressToDelete, '\0', sizeToDelete);
                return true;
            }catch (...){
                std::cout << "[ERROR]: Could not remove record block. \n";
                return false;
            };
        }

        void * loadDataFromDisk(Address address, std::size_t recordSize){
            void *data = operator new(recordSize);
            std::memcpy(data, (char*)startDisk + address.blkNumber * blockSize + address.offset, recordSize);

            return data;
        }

        Address saveDataToDisk(void * itemAddress, std::size_t recordSize){
            Address address = writeRecord(recordSize);
            std::memcpy( (char*)startDisk + address.blkNumber * blockSize + address.offset, itemAddress, recordSize);

            return address;
        }

        bool UpdateDisk(void * itemAddress,  std::size_t recordSize, Address address){
            std::memcpy((char*)startDisk + address.blkNumber * blockSize + address.offset, itemAddress, recordSize);
            return true;
        }

        int memoryUsed(){
            ptrdiff_t sizeUsed = (char *)block + currBlockMemUsed -(char *)startDisk;
            return sizeUsed;
        };

        int currBlockNumber(){
            int blockNum = floor(memoryUsed() / blockSize);
            return blockNum;
        };


};