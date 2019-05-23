#include "basic.h"
#include "Game.h"

#include <cstdlib>
#include <iostream>
using namespace std;

void Game::curses_init() {
#ifndef NO_CURSES
    initscr();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
#endif
}
void Game::game_special0_init(int randSeed, int aiAgent) {
    timer = 1000;
    srand(randSeed);
    map.clear();
    map.newMap();

    // team 0: sankeNum=1,color: green
    // team 1: sankeNum=3,color: red

    teamMgr.newTeam(COLOR_GREEN);
    teamMgr.newTeam(COLOR_RED);
    sankeNum = 4;
    teamMgr.initCURSES_Color();

    pSnakes = new Snake*[sankeNum];
    pSnakes[0] = new Snake(387, R_Act, map, teamMgr.setToTeam(0));
    pSnakes[1] = new Snake(116, D_Act, map, teamMgr.setToTeam(1));
    pSnakes[2] = new Snake(425, D_Act, map, teamMgr.setToTeam(1));
    pSnakes[3] = new Snake(718, D_Act, map, teamMgr.setToTeam(1));

    pAgents = new PolicyMaker*[sankeNum];
    pAgents[0] = new HumanAgent;
    for(int i = 1 ; i < sankeNum ; i++){
        if( aiAgent > 3 ){
            printw("AI Agent could be less then 3 !");
        }
        if( i <= aiAgent){
            pAgents[i] = (*agntsMgr.pAllNewAgentFunc[i-1])();
        }else{
            pAgents[i] = new RandomAgent;
        }
    }

    for (int i = 0; i < sankeNum; ++i) {
        pAgents[i]->setSnake(pSnakes[i]);
        pAgents[i]->linkMap(&map);
        pAgents[i]->linkFoodList(&Flist);
        pAgents[i]->linkpSnakes(pSnakes, sankeNum);
    }
}
void Game::game_standard_init(int randSeed) {  // standard game

    timer = 1000;
    srand(randSeed);
    map.clear();
    map.newMap();

    // team 0: sankeNum=1,color: green
    // team 1: sankeNum=1,color: red

    teamMgr.clear();
    teamMgr.newTeam(COLOR_GREEN);
    teamMgr.newTeam(COLOR_YELLOW);
    teamMgr.newTeam(COLOR_RED);
    teamMgr.newTeam(COLOR_BLUE);
    sankeNum = 4;
    teamMgr.initCURSES_Color();

    pSnakes = new Snake*[sankeNum];
    pSnakes[0] = new Snake(387, R_Act, map, teamMgr.setToTeam(0));
    pSnakes[1] = new Snake(116, D_Act, map, teamMgr.setToTeam(1));
    pSnakes[2] = new Snake(425, D_Act, map, teamMgr.setToTeam(2));
    pSnakes[3] = new Snake(718, D_Act, map, teamMgr.setToTeam(3));

    pAgents = new PolicyMaker*[sankeNum];
    pAgents[0] = (*agntsMgr.pAllNewAgentFunc[0])();
    pAgents[1] = (*agntsMgr.pAllNewAgentFunc[1])();
    pAgents[2] = (*agntsMgr.pAllNewAgentFunc[2])();
    pAgents[3] = (*agntsMgr.pAllNewAgentFunc[3])();

    for (int i = 0; i < sankeNum; ++i) {
        pAgents[i]->setSnake(pSnakes[i]);
        pAgents[i]->linkMap(&map);
        pAgents[i]->linkFoodList(&Flist);
        pAgents[i]->linkpSnakes(pSnakes, sankeNum);
    }
}
void Game::game_delete() {
    teamMgr.clear();
    for (int i = 0; i < sankeNum; ++i) {
        delete pSnakes[i];
        delete pAgents[i];
    }
    delete[] pSnakes;
    delete[] pAgents;
    sankeNum = 0;
}

void Game::plot(int viewL) {  // viewL==0 : global view, else agent0's view
    if (viewL > 0) {
        Map view(map, pSnakes[0]->getPos(), viewL);
        view.print();
    } else {
        map.print();
    }
    printw("\n");
    printw("Time: %d\n", timer);
    /*
    for (int i = 0, s = teamMgr.getTeamCount(); i < s; ++i) {


        if(teamMgr.getSnakeNum(i)>=0)
        printw("Team[%d] has %d Snake(s)\n", i, teamMgr.getSnakeNum(i));
        else  printw("Team[%d] has 0 Snake(s)\n", i, teamMgr.getSnakeNum(i));
    }
     */


    for (int i = 0; i < sankeNum; ++i) {
        printw("Snake%d's length: %d\n", i, pSnakes[i]->getBody().size());
    }

    printw("\n");


    printw("%s", DebuggingMessage.c_str());

#ifdef NO_CURSES
    for (int i = 0; i < 3; ++i)
        printw("\n");
#endif
    refresh();
}
bool Game::update(bool showGame, int viewL) {
    Action keyAction;
#ifndef NO_CURSES
    int keyPress = getch();
#else
    int keyPress = 0;
#endif

    // Snakes' position update
    for (int i = 0; i < sankeNum; ++i) {
        if (pSnakes[i]->isDead()) {
            if (!pSnakes[i]->isCleared())
                teamMgr.teamGet_1_Killed(pSnakes[i]->getTeamID());
            pSnakes[i]->remove(map);
            continue;
        }


        pSnakes[i]->receiveAction(pAgents[i]->actionToDo(keyPress));
        pSnakes[i]->update(map, timer);
    }
    // Check whether snakes die or eat
    for (int i = 0; i < sankeNum; ++i) {
        if (!pSnakes[i]->isDead()) {
            pSnakes[i]->hitWallCheck(map);
            pSnakes[i]->hitSelfCheck();
            for (int j = 0; j < sankeNum; ++j) {
                if (i == j)
                    continue;
                pSnakes[i]->hitSnakeCheck(pSnakes[j]);
            }
        }
    }

    // Foods update
    FoodList::iterator it = Flist.begin();
    while (it != Flist.end()) {
        if (it->eatenCheck(map))
            it = Flist.erase(it);
        else {
            it->update(map);
            ++it;
        }
    }
    // Add new foods
    while (Flist.size() < 100) {
        int newPos;
        while (true) {
            newPos = rand() % 4800;
            if (map[newPos] == ' ')
                break;
        }

        Food newFood(newPos);
        newFood.update(map);
        Flist.push_back(newFood);
    }

    if (showGame)
        plot(viewL);

    // check end of the game?  return toContinue

    int teamsRemain = teamMgr.getTeamCount();
    for (int i = 0, s = teamsRemain; i < s; ++i)
        if (teamMgr.getSnakeNum(i) <= 0)
            --teamsRemain;

    //printw("teamsRemain: %d\n", teamsRemain);
    if (teamsRemain <= 0)
        return false;
    else {
        --timer;
        return timer > 0;
    }

}

void Game::humanGame(int randSeed, int aiAgent, int viewL) {
    game_special0_init(randSeed, aiAgent);
    playing(true, viewL);
    showEndingMsg();
    game_delete();
}
void Game::singleGame(int randSeed, bool showGame) {
    game_standard_init(randSeed);
    playing(showGame);
    showEndingMsg();
    game_delete();
}
void Game::battleAll(const char* dumpFileName, bool showGame) {
static int gameStageCount = 10;
static int gameStage[10] = {1, 2, 6, 24, 120, 720, 5040 ,362880 ,3628800, 39916800};  // 1543

	int size = agntsMgr.pAllNewAgentFunc.size();

	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
		    agntsMgr.scores[i][j] = 0;



	for (int k = 0; k < gameStageCount; ++k) {
		game_standard_init( gameStage[k]);
		playing(showGame);



		for( int i = 0 ; i < 4 ; i++){
			int point = pSnakes[i]->_finalBodyLength*10;
			agntsMgr.agentName.push_back(pAgents[i]->getName());
			agntsMgr.scores[i][i] += pSnakes[i]->_finalBodyLength * 10;
			printw("Snake %d : %d points\n",i,agntsMgr.scores[i][i]);
		}

		//printw("Please press q to exit\n");
		//while( getch()!='q' ){}

	}
	agntsMgr.dump(dumpFileName);
}

void Game::showEndingMsg() {
    int win = -1;
    int score_p0 = 0, score_p1 = 0;

    for (int i = 0, s = teamMgr.getTeamCount(); i < s; ++i) {
        if (teamMgr.getSnakeNum(i) > 0) {
            win = i;
            break;
        }
    }
    /*
    if (timer <= 0)
        win = -1;
    if (teamMgr.getSnakeNum(0) > 0)
        score_p0 = (timer <= 0) ? 400 : (900 + timer);
    if (teamMgr.getSnakeNum(1) > 0)
        score_p1 = (timer <= 0) ? 400 : (900 + timer);
     */
    score_p0 += pSnakes[0]->_finalBodyLength * 10;
    score_p1 += pSnakes[1]->_finalBodyLength * 10;

    //if (win == -1)
    //    printw("Match game!!\t(team 0: %d p;team 1: %d p)\n", score_p0,
    //           score_p1);
    //else
        printw("team %d wins!!\t(team 0: %d p;team 1: %d p)\n", win, score_p0, score_p1);
}
