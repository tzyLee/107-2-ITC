#include <cstdlib>
#include <ctime>
#include <cwchar>

#include "Game.h"
Py_Object* module;
int main() {
    Py_Initialize();
    PySys_SetPath(L"TODO: absolute path to current dir");
    PyObject* str = PyUnicode_FromString("evolution_strategy");
    if (!str) {
        PyErr_Print();
        std::cerr << "Error creating string\n";
        return -1;
    }
    module = PyImport_Import(str);
    if (!module) {
        PyErr_Print();
        std::cerr << "Error importing module\n";
        return -1;
    }
    Py_DECREF(str);
    Game game;
    // TA: choose one of the following mode to start the game:
    // humanGame,singleGame,battleAll you are the green snake

    game.singleGame(4, false);
    Py_DECREF(module);
    // game.humanGame(4,0,0);
    // TA: parameters: int randSeed=4,int aiAgent=0,int viewL=0

    // game.battleAll("test.csv", true);
    // TA: parameters: const char* dumpFileName,bool showGame=false

#ifndef NO_CURSES
    printw("Please press q to exit\n");
    while (getch() != 'q') {
    }
#endif
    Py_Finalize();
    return 0;
}
