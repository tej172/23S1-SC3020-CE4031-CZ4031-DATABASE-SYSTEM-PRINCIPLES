#include "memory_pool.h"
#include "types.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <cstring>

// Constructor
MemoryPool::MemoryPool(std::size_t maxMemoryPoolSize, std::size_t blkFixedSize)
{
  // Initialize member variables with provided values
  this->maxMemoryPoolSize = maxMemoryPoolSize;          // Maximum size allowed for the memory pool.
  this->blkFixedSize = blkFixedSize;                // Size of each fixed-size block in the pool.
  this->totalSizeUsed = 0;                              // Total size of memory used in the memory pool.
  this->actualDataSizeUsed = 0;                         // Actual data size used based on records stored in storage.
  this->totalBlksAllocated = 0;                       // Number of currently allocated blocks.

  // Create pool of blocks using operator new
  this->memoryPool = operator new(maxMemoryPoolSize);   // Allocate a continuous block of memory with maxMemoryPoolSize.
  std::memset(memoryPool, '\0', maxMemoryPoolSize);     // Initialize the entire pool to null (set all bytes to null).
  this->currentBlk = nullptr;                         // Set the current block pointer to nullptr.
  this->currentBlkSizeUsed = 0;                       // Initialize the size used within the current block to 0.
  this->totalBlksAccessed = 0;                        // Initialize the count of blocks accessed to 0.
}

//Methods:
// 1. Allocate a block
bool MemoryPool::allocateBlock()
{
  // Only allocate a new block if we don't exceed maxMemoryPoolSize.
  if (totalSizeUsed + blkFixedSize <= maxMemoryPoolSize)
  {
    // Update variables
    totalSizeUsed += blkFixedSize;
    currentBlk = (char *)memoryPool + totalBlksAllocated * maxMemoryPoolSize; // Calculates the memory address for the new block within the memory pool
    currentBlkSizeUsed = 0;           // Keep track of how much space is used within current block. Since we are starting with a new block, it's set to 0
    totalBlksAllocated += 1;          // keep track of the number of blocks allocated so far
    return true;
  }
  else
  {
    std::cout << "Error: No memory left to allocate new block (" << totalSizeUsed << "/" << maxMemoryPoolSize << " used)." << '\n';
    return false;
  }
}

// 2. Allocate records
Address MemoryPool::allocateRecords(std::size_t sizeRequired)
{
  // Check if the size required for a record exceeds the fixed block size.
  if (sizeRequired > blkFixedSize)
  {
    // Display an error message indicating that the requested size is too large.
    std::cout << "Error: Size required for a record is larger than the block size (" << sizeRequired << " vs " << blkFixedSize << ")! Increase block size to store data." << '\n';

    // Throw an exception to indicate that the allocation failed due to the oversized record.
    throw std::invalid_argument("Requested size too large!");
  }

  // Check if there is no current block or if the record cannot fit into the current block.
  if (totalBlksAllocated == 0 || (currentBlkSizeUsed + sizeRequired > blkFixedSize))
  {
    // If a new block is needed, attempt to allocate one.
    bool isSuccessful = allocateBlock();

    // If allocation fails, throw an exception.
    if (!isSuccessful)
    {
      throw std::logic_error("Failed to allocate a new block!");
    }
  }

  // Calculate the offset within the current block where the record will be placed.
  short int offsetWithinBlock = currentBlkSizeUsed;

  // Update the size used within the current block to include the newly allocated record.
  currentBlkSizeUsed += sizeRequired;

  // Update the total actual data size used in the memory pool to track cumulative data size.
  actualDataSizeUsed += sizeRequired;

  // Create an address structure representing the location where the record will be stored.
  Address recordAddress = {currentBlk, offsetWithinBlock};

  // Return the address to indicate where the record should be placed in the memory pool.
  return recordAddress;
}

// 3. Deallocate records
bool MemoryPool::deallocateRecords(Address recordAddress, std::size_t sizeToDelete)
{
  try
  {
    // Calculate the memory address of the record to delete within its block.
    void *recordToDeleteAddress = (char *)recordAddress.blkAddress + recordAddress.offset;

    // Set the memory of the record to delete to null (remove data).
    std::memset(recordToDeleteAddress, '\0', sizeToDelete);

    // Update the actual size used in the memory pool to reflect the removed record.
    actualDataSizeUsed -= sizeToDelete;

    // Check if the block is empty by creating a test block filled with null bytes.
    unsigned char testBlock[blkFixedSize];
    memset(testBlock, '\0', blkFixedSize);

    // Compare the content of the actual block with the test block to check if it's empty.
    if (memcmp(testBlock, recordAddress.blkAddress, blkFixedSize) == 0)
    {
      // If the block is empty, reduce the total size used in the memory pool.
      totalSizeUsed -= blkFixedSize;

      // Decrease the count of allocated blocks.
      totalBlksAllocated = totalBlksAccessed - 1;
    }

    // Return true to indicate successful deallocation.
    return true;
  }
  catch (...)
  {
    // If any error occurs during deallocation, print an error message.
    std::cout << "Error: Could not remove record/block at given address (" << recordAddress.blkAddress << ") and offset (" << recordAddress.offset << ")." << '\n';

    // Return false to indicate unsuccessful deallocation.
    return false;
  }
}


// 4.1 Save data into disk (by creating new blocks)
// Saves data into the disk. Returns the disk address.
// (new place, old one)
Address MemoryPool:: saveData(void *dataToSaveAddress, std::size_t dataSize)
{
  // Allocate a block in the memory pool for the data.
  Address diskDataAddress = allocateRecords(dataSize);

  // Copy the data from the provided memory address to the allocated disk data address.
  std::memcpy((char *)diskDataAddress.blkAddress + diskDataAddress.offset, dataToSaveAddress, dataSize);

  // Update the count of blocks accessed.
  totalBlksAccessed = totalBlksAccessed + 1;

  // Return the disk address where the data has been saved.
  return diskDataAddress;
}

// 4.2 Save data by updating the previous data in the disk
// Update data in the disk if it has already been saved before.
Address MemoryPool::saveData(void *dataToSaveAddress, std::size_t dataSize, Address diskDataAddress)
{
  // Copy the updated data from the provided memory address to the specified disk data address.
  std::memcpy((char *)diskDataAddress.blkAddress + diskDataAddress.offset, dataToSaveAddress, dataSize);

  // Update the count of blocks accessed.
  totalBlksAccessed = totalBlksAccessed + 1;

  // Return the same disk address indicating where the data has been updated.
  return diskDataAddress;
}

// 5. Load data form disk
// Give a block address, offset, and size; return the data located there.
void *MemoryPool::loadData(Address dataAddress, std::size_t dataSize)
{
  // Allocate memory in the main memory pool to store the loaded data.
  void *mainMemoryData = operator new(dataSize);

  // Copy the data from the specified block address and offset to the allocated main memory.
  std::memcpy(mainMemoryData, (char *)dataAddress.blkAddress + dataAddress.offset, dataSize);

  // Update the count of blocks accessed.
  totalBlksAccessed = totalBlksAccessed + 1;

  // Return the main memory address where the data has been loaded.
  return mainMemoryData;
}


MemoryPool::~MemoryPool(){};
