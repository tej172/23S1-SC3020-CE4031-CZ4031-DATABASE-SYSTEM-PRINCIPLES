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
    ifstream inputFile("Data/games.txt");
	
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }
	string line;
	while (getline(inputFile, line)) {
		// Split the line into fields
		char* token = strtok(const_cast<char*>(line.c_str()), "\t");
		recordStruct record;
		bool firstLine = true;
		
		// Parse and store data in the recordStruct
		for (size_t i = 0; i < recordStruct::NUM_FIELDS && token; ++i) {
			if (firstLine) continue;
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
		std::cout << record.FG_PCT_home << "\n";
		Address address = disk.saveDataToDisk(&record, sizeof(recordStruct));

		BPtree.insert(record.FG_PCT_home, address);
	}
	std::cout << "the record count is: " << countRecord << "\n";
	vector<Address> res = BPtree.findKeyRange(0.5, 0.5);
	std::cout << "the record count is: " << res.size();

	for (int i=0; i< res.size(); i++){
		void* data = disk.loadDataFromDisk(res[i], sizeof(recordStruct));
		recordStruct* loadedRecord = static_cast<recordStruct*>(data);
		if (loadedRecord->FG_PCT_home != 0.5){
			std::cout << "ERROR";
			std::cout << "Data from the record: " << loadedRecord->FG_PCT_home;
		}
	}

	return 0;
}

#endif