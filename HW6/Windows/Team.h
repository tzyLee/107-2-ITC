#ifndef __Team_h__
#define __Team_h__

#include "basic.h"
#include <vector>

class TeamsManager;
class Team {
   private:
    int teamID;
    int curses_color;

   public:
    Team(int tID, int color = COLOR_WHITE) : teamID(tID), curses_color(color) {}
    ~Team() {}
    friend class TeamsManager;
    int getTeamID() const { return teamID; }
    int colorChar(char rawChar) const {
#ifndef NO_CURSES
        return (rawChar | COLOR_PAIR(teamID + 1));
#else
        return rawChar;
#endif
    }
};

class TeamsManager {
   private:
    int teamCount;
    std::vector<Team> Teams;
    std::vector<int> aliveNum;

    int addTeam(int curses_color, int tankNum = 1) {
        int teamID = teamCount;
        Teams.push_back(Team(teamID, curses_color));
        aliveNum.push_back(tankNum);
        ++teamCount;
        return teamID;
    }

   public:
    TeamsManager() { teamCount = 0; }
    int newTeam(int curses_color) { return addTeam(curses_color, 0); }
    Team setToTeam(int teamID) {
        ++aliveNum[teamID];
        return Teams[teamID];
    }
    void clear() {
        Teams.clear();
        aliveNum.clear();
        teamCount = 0;
    }

    int getTeamCount() const { return teamCount; }
    Team getTeam(int teamID) const { return Teams[teamID]; }
    int getSnakeNum(int teamID) const { return aliveNum[teamID]; }
    void teamGet_1_Killed(int teamID) { --aliveNum[teamID]; }
    void initCURSES_Color() {  // called after Teams added
#ifndef NO_CURSES
        start_color();
        for (int teamID = 0; teamID < teamCount; ++teamID) {
            init_pair(teamID + 1, Teams[teamID].curses_color, COLOR_BLACK);
        }
#endif
    }
};

#endif  // __Team_h__
