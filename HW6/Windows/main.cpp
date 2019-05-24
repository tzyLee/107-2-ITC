#include<cstdlib>
#include<ctime>

#include "Game.h"

int main(){
    Game game;
    // TA: choose one of the following mode to start the game: humanGame,singleGame,battleAll
    // you are the green snake
	
    //game.humanGame(4,0,0);
        // TA: parameters: int randSeed=4,int aiAgent=0,int viewL=0

    game.battleAll("test.csv",true);
        // TA: parameters: const char* dumpFileName,bool showGame=false

    #ifndef NO_CURSES
    printw("Please press q to exit\n");
    while( getch()!='q' ){}
    #endif

    return 0;
}
