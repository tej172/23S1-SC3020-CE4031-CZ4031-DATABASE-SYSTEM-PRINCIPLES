#include <iostream>
#include "memory_pool.h"
#include "types.h"

using namespace std;

int main()
{
    MemoryPool mp(200000, 400);


    string str("test");

    Address Add = mp.saveData(&str, sizeof(str));

    void* loadedMemory = mp.loadData(Add, sizeof(str));
    cout << str<< endl;

    cout << loadedMemory << endl;

    return 0;
}
