#include <cassert>
#include <cstdlib>
#include <ctime>
#include "Game.h"

const char* pythonObjectID = nullptr;
int main(int argc, char* argv[]) {
    assert(argc > 1);
    pythonObjectID = argv[1];
    Game game;
    // TA: choose one of the following mode to start the game:
    // humanGame,singleGame,battleAll you are the green snake

    // game.humanGame(4,0,0);
    // TA: parameters: int randSeed=4,int aiAgent=0,int viewL=0
    char filename[100] = "agents/";
    strcat(filename, pythonObjectID);
    game.battleAll(filename, false);
    // TA: parameters: const char* dumpFileName,bool showGame=false

#ifndef NO_CURSES
    // printw("Please press q to exit\n");
    // while (getch() != 'q') {
    // }
#endif

    return 0;
}
