#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "util/Address.h"
#include "util/recordStruct.h"
#include "Disk/Disk.h"
#include "Disk/Disk.cpp"
#include "BPlusTree/BPlusTree.h" // Include the B+ tree header
#include <chrono> // Include the chrono library

using namespace std;

// Function to calculate the size of a record
size_t calculateRecordSize(const recordStruct& record) {
    size_t recordSize = 0;
    for (size_t i = 0; i < recordStruct::NUM_FIELDS; ++i) {
        switch (i) {
            case 0:
                recordSize += record.GAME_DATE_EST.size();
                break;
            case 1:
            case 2:
                recordSize += sizeof(int);
                break;
            case 3:
            case 4:
            case 5:
                recordSize += sizeof(float);
                break;
            case 6:
            case 7:
                recordSize += sizeof(int);
                break;
            case 8:
                recordSize += sizeof(bool);
                break;
            default:
                break;
        }
    }
    return recordSize;
}

int main() {
    // Initialize statistics
    int numRecords = 0;
    size_t totalRecordSize = 0;
    int recordsPerBlock = 0;
    int numBlocks = 0;
    
    // Initialize the Disk object with your desired block and disk size
    Disk disk(400, 300000000);

    ifstream inputFile("Data/games.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    // Create a B+ tree for Experiment 2
    BPlusTree<float> BPtree = BPlusTree<float>(6);

    // Read and process each line from the input file
    string line;
    while (getline(inputFile, line)) {
        // Split the line into fields
        char* token = strtok(const_cast<char*>(line.c_str()), "\t");
        recordStruct record;

        // Parse and store data in the recordStruct
        for (size_t i = 0; i < recordStruct::NUM_FIELDS && token; ++i) {
            switch (i) {
                case 0: // GAME_DATE_EST
                    record.GAME_DATE_EST = token;
                    break;
                case 1: // TEAM_ID_home
                    record.TEAM_ID_home = atoi(token);
                    break;
                case 2: // PTS_home
                    record.PTS_home = atoi(token);
                    break;
                case 3: // FG_PCT_home
                    record.FG_PCT_home = atof(token);
                    break;
                case 4: // FT_PCT_home
                    record.FT_PCT_home = atof(token);
                    break;
                case 5: // FG3_PCT_home
                    record.FG3_PCT_home = atof(token);
                    break;
                case 6: // AST_home
                    record.AST_home = atoi(token);
                    break;
                case 7: // REB_home
                    record.REB_home = atoi(token);
                    break;
                case 8: // HOME_TEAM_WINS
                    record.HOME_TEAM_WINS = atoi(token);
                    break;
                default:
                    break;
            }
            token = strtok(nullptr, "\t");
        }

        // Calculate the size of the record
        size_t recordSize = calculateRecordSize(record);

        

        // Check if there's enough space in the current block for the next record
        if (recordSize > disk.getBlockSize() - disk.getCurrBlockMemUsed()) {
            // Allocate a new block when there's not enough space
            if (!disk.allocateOneBlock()) {
                cerr << "Error: Failed to allocate a new block." << endl;
                return 1;
            }
            ++numBlocks; // Increment the block count
            recordsPerBlock = 1;
        }

        // Store the record on the disk
        Address address = disk.saveDataToDisk(&record, recordSize);

        // Insert "FG_PCT_home" into the B+ tree
        // bPlusTree.insert(address, record.FG_PCT_home);
		BPtree.insert(record.FG_PCT_home, address);
        // Update statistics
        ++numRecords;
        totalRecordSize += recordSize;
        ++recordsPerBlock;
    }

    // Calculate the final statistics
    double averageRecordSize = static_cast<double>(totalRecordSize) / numRecords;

    // Report the statistics for Experiment 1
    cout << "Experiment 1 Statistics:" << endl;
    cout << "Number of Records: " << numRecords << endl;
    cout << "Size of a Record (bytes): " << averageRecordSize << endl;
    cout << "Number of Records Stored in a Block: " << recordsPerBlock << endl;
    cout << "Number of Blocks for Storing the Data: " << numBlocks << endl;

    // Report the statistics for Experiment 2
if (numRecords > 0) {
    // Print B+ tree statistics
    cout << "Experiment 2 Statistics (B+ Tree):" << endl;
    // cout << "Parameter 'n' of the B+ Tree: " << bPlusTree.getN() << endl;
    // cout << "Number of Nodes in the B+ Tree: " << bPlusTree.getNumNodes() << endl;
    // cout << "Number of Levels in the B+ Tree: " << bPlusTree.getNumLevels() << endl;

    cout << "Content of the Root Node (only keys): ";
    // bPlusTree.printRoot();
    cout << endl;
} else {
    cout << "No records have been inserted into the B+ tree." << endl;
}



vector<Address> res = BPtree.findKeyRange(0.5, 0.5);
	std::cout << "the record count is: " << res.size();

	for (int i=0; i< res.size(); i++){
		void* data = disk.loadDataFromDisk(res[i], sizeof(recordStruct));
		recordStruct* loadedRecord = static_cast<recordStruct*>(data);
		if (loadedRecord->FG_PCT_home == 0.5){
			std::cout << "True";
			std::cout << "Data from the record: " << loadedRecord->FG_PCT_home;
		}
	}

	return 0;
    // Experiment 3: Retrieve movies with "FG_PCT_home" equal to 0.5


// std::vector<Address*> resultAddresses = bPlusTree->findSearchKey(searchKey, numIndexNodesAccessed);

// // Print the addresses found
// cout << "Addresses with FG_PCT_home equal to " << searchKey << ":" << endl;
// for (Address* address : resultAddresses) {
//     cout << "Block Number: " << address->blkNumber << ", Offset: " << address->offset << endl;
// }

    // float searchKey = 0.5f;
    // int numIndexNodesAccessed = 0;
    // int numDataBlocksAccessed = 0;

    // Start measuring the execution time
    // auto startTime = chrono::high_resolution_clock::now();
     
    // int resultAddresses = bPlusTree.findKeyRange(searchKey,searchKey );

    // cout << "resultAddresses: " << resultAddresses << endl;
    // Stop measuring the execution time
    // auto endTime = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    // // If the search was successful, retrieve the associated record
    // if (resultAddresses != nullptr) {
    //     recordStruct* resultRecord = static_cast<recordStruct*>(disk.loadDataFromDisk(*resultAddresses, recordSize));

    //     // Calculate the running time in milliseconds
    //     double runningTime = static_cast<double>(duration.count()) / 1000.0;

    //     // Report the results
    //     cout << "\nExperiment 3 Statistics (B+ Tree Retrieval):" << endl;
    //     cout << "Number of Index Nodes Accessed: " << numIndexNodesAccessed << endl;
    //     // cout << "Number of Data Blocks Accessed: " << numDataBlocksAccessed << endl;
    //     cout << "Average FG3_PCT_home of Retrieved Records: " << resultRecord->FG3_PCT_home << endl;
    //     cout << "Running Time of B+ Tree Retrieval (ms): " << runningTime << endl;

    //     // Clean up
    //     delete resultRecord;
    // } else {
    //     cout << "\nNo records found with FG_PCT_home equal to " << searchKey << endl;
    // }
  
}