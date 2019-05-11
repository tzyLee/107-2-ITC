#include <iostream>
#include <fstream>

using namespace std;

#include "heap.h"

int main()
{

    Heap<int, char> heap;

    ifstream file("input.txt");

    int opNum;
    int heapNum;
    file >> opNum >> heapNum;
    int *key = new int[heapNum];
    char *element = new char[heapNum];

    for (int i=0; i<heapNum; ++i) {
        file >> key[i];
        file >> element[i];
    }
    int index = 0;
    for (int i=0; i<opNum; ++i) {
        int op;
        file >> op;
        if (op == 1) {
            heap.push(key[index], element[index]);
            ++index;
        }
        else 
            heap.pop();
    }

    file.close();
    delete []element;
    delete []key;

    heap.printArray();

    heap.printByPopping();

    return 0;
}


