#ifndef BPlusTree_H
#define BPlusTree_H

#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "BPlusTree/BPlusTree.h"
#include "util/recordStruct.h"
#include "util/data_utils.cpp"
#include "Disk/Disk.h"
using namespace std;


int main(){
	Disk disk = Disk(400, 300000000);

	BPlusTree<float> BPtree = BPlusTree<float>(6);
	int countRecord = 0;
	float smallestRecord = 10000;
    ifstream inputFile("Data/games.txt");
	
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }
	string line;
	bool firstLine = true;
	
	while (getline(inputFile, line)) {
		// Split the line into fields
		char* token = strtok(const_cast<char*>(line.c_str()), "\t");
		recordStruct record;
		
		if (firstLine){
			firstLine = false;
			continue;
		}
		
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

		//std::cout << record.FG_PCT_home << "\n";
		if (record.FG_PCT_home == 0) std::cout << "the line with 0: ";
		if (record.FG_PCT_home < smallestRecord) smallestRecord = record.FG_PCT_home;


		Address address = disk.saveDataToDisk(&record, sizeof(recordStruct));

		BPtree.insert(record.FG_PCT_home, address);
	}
	std::cout << "the smallest record is: " << smallestRecord << "\n";
	vector<Address> res = BPtree.findKeyRange(0, 0.3);
	std::cout << "the record count is: " << res.size();

	

	Node<float> * val_temp = BPtree.findFirstMostNode();
	std::cout << "\nLEFTMOST value:: " << val_temp->key[0];

	int numOfDeleted =  BPtree.delKeyRange(0, 0.3);

	std::cout << "\n No. of deleted records are: " << numOfDeleted << "\n";

	vector<Address> resAfter = BPtree.findKeyRange(0, 0.3);
	std::cout << "AFTER DELETE ::: the record count is: " << resAfter.size();

	return 0;
}

#endif