#include "Simulator.h"
#include <string>
#include <sstream>

int main() {
    Simulator sim;
    for(int i = 1 ; i < 7 ; i++){
	string index;
	std::stringstream ss;
	ss << i;
	index = ss.str();
        sim.loadMemory("input/input"+index); 
        sim.simulate();
        sim.storeMemory("output/output"+index);
    }

    return 0;
}
