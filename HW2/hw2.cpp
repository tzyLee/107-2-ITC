#include "Simulator.h"
#include <string>

int main() {
    Simulator sim;
    for(int i = 1 ; i < 7 ; i++){
        std::string index = std::to_string(i);
        sim.loadMemory("input/input"+index); 
        sim.simulate();
        sim.storeMemory("output/output"+index);
    }
    sim.loadAssembly("input/bonus.txt");
    sim.simulate();
    sim.storeMemory("output/bonus");
    return 0;
}
