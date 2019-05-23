#ifndef __Game_h__
#define __Game_h__

#include "basic.h"
#include "Map.h"
#include "Food.h"
#include "Snake.h"
#include "Team.h"
#include "PolicyMaker.h"
#include "AgentsMgr.h"
#include <vector>

class Game {
   private:
    Map map;
    Snake** pSnakes;
    PolicyMaker** pAgents;
    FoodList Flist;
    TeamsManager teamMgr;
    AgentsMgr agntsMgr;
    int sankeNum;
    int timer;

    void curses_init();
    void game_special0_init(int randSeed = 4, int aiAgent = 0);
    void game_standard_init( int randSeed = 4);
    void game_delete();

    void plot(int viewL);  // viewL==0 : global view, else agent0's view
    bool update(bool showGame, int viewL = 0);
    bool playing(bool showGame = true, int viewL = 0) {
        if (showGame) {
            plot(viewL);
            while (update(showGame, viewL)) {
                napms(40);
            }
        } else {
            while (update(showGame)) {
            }
        }
        return true;
    }

   public:
    Game() { curses_init(); }
    ~Game() {
#ifndef NO_CURSES
        endwin();
#endif
    }

    void humanGame(int randSeed = 4, int aiAgent = 0, int viewL = 0);
    void singleGame(int randSeed = 4,
                    bool showGame = true);
    void battleAll(const char* dumpFileName, bool showGame = false);
    void showEndingMsg();
};

#endif  // __Game_h__
