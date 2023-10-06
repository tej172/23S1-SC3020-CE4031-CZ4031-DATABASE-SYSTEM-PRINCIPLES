// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstring>
// #include <ctime>
// #include <cstdlib>
// #include "Address.h" 
// #include "recordStruct.h" 
// #include "Disk.h"
// #include "../MemoryPool/Disk.cpp"

// using namespace std;

// // Define the size of each field in bytes
// const size_t FIELD_SIZES[] = {
//     10, // GAME_DATE_EST
//     4,  // TEAM_ID_home
//     4,  // PTS_home
//     4,  // FG_PCT_home
//     4,  // FT_PCT_home
//     4,  // FG3_PCT_home
//     4,  // AST_home
//     4,  // REB_home
//     1   // HOME_TEAM_WINS
// };

// const size_t NUM_FIELDS = sizeof(FIELD_SIZES) / sizeof(FIELD_SIZES[0]);

// // Function to calculate the size of a record
// size_t calculateRecordSize() {
//     size_t recordSize = 0;
//     for (size_t i = 0; i < NUM_FIELDS; ++i) {
//         recordSize += FIELD_SIZES[i];
//     }
//     return recordSize;
// }

// int main() {
//     // Initialize statistics
// int numRecords = 0;
// size_t totalRecordSize = 0;
// int recordsPerBlock = 0;
// int numBlocks = 0;

// // Initialize the Disk object with your desired block and disk size
// Disk disk(400, 300000000);


// ifstream inputFile("../src/games.txt");
// if (!inputFile.is_open()) {
//     cerr << "Error: Unable to open input file." << endl;
//     return 1;
// }

// // Read and process each line from the input file
// string line;
// while (getline(inputFile, line)) {
//     // Split the line into fields
//     char* token = strtok(const_cast<char*>(line.c_str()), "\t");
//     recordStruct record;

//     // Parse and store data in the recordStruct
//     for (size_t i = 0; i < NUM_FIELDS && token; ++i) {
//         switch (i) {
        
//             case 0: // GAME_DATE_EST
//                 record.GAME_DATE_EST = token;
//                 break;
//             case 1: // TEAM_ID_home
//                 record.TEAM_ID_home = atoi(token);
//                 break;
//             case 2: // PTS_home
//                 record.PTS_home = atoi(token);
//                 break;
//             case 3: // FG_PCT_home
//                 record.FG_PCT_home = atof(token);
//                 break;
//             case 4: // FT_PCT_home
//                 record.FT_PCT_home = atof(token);
//                 break;
//             case 5: // FG3_PCT_home
//                 record.FG3_PCT_home = atof(token);
//                 break;
//             case 6: // AST_home
//                 record.AST_home = atoi(token);
//                 break;
//             case 7: // REB_home
//                 record.REB_home = atoi(token);
//                 break;
//             case 8: // HOME_TEAM_WINS
//                 record.HOME_TEAM_WINS = atoi(token);
//                 break;
//             default:
//                 break;
//         }
//         token = strtok(nullptr, "\t");
//     }

//     // Calculate the size of the record
//     size_t recordSize = calculateRecordSize();

//     // Check if there's enough space in the current block for the next record
//     if (recordSize > disk.getBlockSize() - disk.getCurrBlockMemUsed()) {
//         // Allocate a new block when there's not enough space
//         if (!disk.allocateOneBlock()) {
//             cerr << "Error: Failed to allocate a new block." << endl;
//             return 1;
//         }
//         ++numBlocks; // Increment the block count
//         recordsPerBlock = 1; 
//     }

//     // Store the record on the disk
//     Address address = disk.saveDataToDisk(&record, recordSize);

//     // Update statistics
//     ++numRecords;
//     totalRecordSize += recordSize;
//     ++recordsPerBlock;
// }

//     // Calculate the final statistics
//     double averageRecordSize = static_cast<double>(totalRecordSize) / numRecords;

//     // Report the statistics
//     cout << "Number of Records: " << numRecords << endl;
//     cout << "Size of a Record (bytes): " << averageRecordSize << endl;
//     cout << "Number of Records Stored in a Block: " << recordsPerBlock << endl;
//     cout << "Number of Blocks for Storing the Data: " << numBlocks << endl;

//     return 0;
// }


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "Address.h"
#include "recordStruct.h"
#include "Disk.h"
#include "../MemoryPool/Disk.cpp"


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

    ifstream inputFile("../src/games.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }



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



  
}
