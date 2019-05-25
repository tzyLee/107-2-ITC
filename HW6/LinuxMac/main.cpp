#include <cassert>
#include <cstdlib>
#include <ctime>

#include "Game.h"
PyObject* module;
char* nnfilename;
int main(int argc, char* argv[]) {
    assert(argc > 1);
    nnfilename = argv[1];
    Py_Initialize();
    PySys_SetPath(
        L"/usr/lib/python36.zip:/usr/lib/python3.6:/usr/lib/python3.6/"
        L"lib-dynload:/usr/local/lib/python3.6/dist-packages:/usr/lib/"
        L"python3.6/dist-packages:/usr/lib/python3/dist-packages:/home/"
        L"pythagordes/Desktop/IntroToCS/HW6/"
        L"LinuxMac");
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

    // game.singleGame(4, false);
    // game.humanGame(4, 0, 0);
    // TA: parameters: int randSeed=4,int aiAgent=0,int viewL=0

    game.battleAll("test.csv", false);
    // TA: parameters: const char* dumpFileName,bool showGame=false

#ifndef NO_CURSES
    printw("Please press q to exit\n");
    while (getch() != 'q') {
    }
#endif
    Py_DECREF(module);
    Py_Finalize();
    return 0;
}
