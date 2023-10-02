#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "types.h"

#include <vector>
#include <unordered_map>
#include <tuple>

class MemoryPool
{
public:
  // =============== Methods ================ //

  // Constructor: Creates a new memory pool with the specified maximum size and block size.
  MemoryPool(std::size_t maxMemoryPoolSize, std::size_t blkFixedSize);

  // Allocate a new block from the memory pool. Returns false if an error occurs.
  bool allocateBlock();

  // Allocate a new chunk of memory in the memory pool. Creates a new block if necessary.
  Address allocateRecords(std::size_t sizeRequired);

  // Deallocate an existing record and block if the block becomes empty. Returns false if an error occurs.
  bool deallocateRecords(Address recordAddress, std::size_t sizeToDelete);

  // Save data to the memory pool given a main memory address. Allocates a new block if necessary.
  Address saveData(void *dataToSaveAddress, std::size_t dataSize);

  // Update data in the memory pool if it has already been saved before.
  Address saveData(void *dataToSaveAddress, std::size_t dataSize, Address diskDataAddress);

  // Load data from the memory pool based on the given block address, offset, and size.
  void *loadData(Address dataAddress, std::size_t dataSize);

  // Returns the maximum size of this memory pool.
  std::size_t getMaxPoolSize() const
  {
    return maxMemoryPoolSize;
  }

  // Returns the size of a block in the memory pool.
  std::size_t getBlockSize() const
  {
    return blkFixedSize;
  };

  // Returns the size used in the current block.
  std::size_t getBlockSizeUsed() const
  {
    return currentBlkSizeUsed;
  };

  // Returns the current size used in the memory pool (total block size).
  std::size_t getSizeUsed() const
  {
    return totalSizeUsed;
  }

  // Returns the actual size of all records stored in the memory pool.
  std::size_t getActualSizeUsed() const
  {
    return actualDataSizeUsed;
  }

  // Returns the number of currently allocated blocks.
  int getAllocatedBlocks() const
  {
    return totalBlksAllocated;
  };

  // Returns the number of blocks accessed.
  int getBlocksAccessed() const
  {
    return totalBlksAccessed;
  }

  // Resets the count of blocks accessed and returns the previous count.
  int resetBlocksAccessed()
  {
    int tempBlocksAccessed = totalBlksAccessed;
    totalBlksAccessed = 0;
    return tempBlocksAccessed;
  }

  // Destructor
  ~MemoryPool();

private:
  // =============== Data ================ //

  std::size_t maxMemoryPoolSize;    // Maximum size allowed for the memory pool.
  std::size_t blkFixedSize;      // Size of each block in pool in bytes.
  std::size_t totalSizeUsed;       // Current size used up for storage (total block size).
  std::size_t actualDataSizeUsed; // Actual size used based on records stored in storage.
  std::size_t currentBlkSizeUsed;  // Size used up within the current block we are pointing to.

  int totalBlksAllocated;      // Number of currently allocated blocks.
  int totalBlksAccessed; // Counts the number of blocks accessed.

  void *memoryPool;  // Pointer to the memory pool.
  void *currentBlk; // Current block pointer we are inserting to.
};

#endif
