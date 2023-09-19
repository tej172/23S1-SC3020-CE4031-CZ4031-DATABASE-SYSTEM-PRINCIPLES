//============================================================================
// Name        : database.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include "MemoryPool/MemoryPool.cpp"

#include <string>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	std::string data1 = "hello";
	std::string data2 = "testingtesting";

	Disk d(400, 300000000);
	Address add1 = d.saveDataToDisk(&data1, sizeof(std::string));
	Address add2 = d.saveDataToDisk(&data2, sizeof(std::string));

	void * fromDiskdata1 = d.loadDataFromDisk(add1, sizeof(std::string) );
	void * fromDiskdata2 = d.loadDataFromDisk(add2, sizeof(std::string) );

	std::string* strPtr = static_cast<std::string*>(fromDiskdata1);
	std::cout << *strPtr << std::endl;

	std::string* strPtr2 = static_cast<std::string*>(fromDiskdata2);
	std::cout << *strPtr2 << std::endl;


	puts("Hello World!!!");
	return EXIT_SUCCESS;
}
