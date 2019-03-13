#ifndef __SIMULATOR__
#define __SIMULATOR__

#include <string>
using namespace std;

class Simulator{
public:
    bool loadMemory(string);
    bool storeMemory(string);
    bool simulate();
};

#endif
