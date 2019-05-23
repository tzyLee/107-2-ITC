#ifndef __PolicyMaker_h__
#define __PolicyMaker_h__

#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "basic.h"
#include "Map.h"
#include "Snake.h"
#include "Food.h"

extern std::string DebuggingMessage;

class PolicyMaker {
   private:
    Map* theMap;
    FoodList* theFoods;
    Snake** pSnakes;
    int snakeNum;
    char name[32];

   protected:
    const Snake* pSnake;
    Map view;
    std::vector<Food> FoodinView;
    // The position of snakes in SnakeinView is recorded in "theMap", not in "view".
    std::vector<Snake> SnakeinView;

    void pos2xy(const int pos, int& x, int& y) {
        x = pos % theMap->getWidth();
        y = pos / theMap->getWidth();
    }

    void getView(int rad);
    void getFoodInView(int rad);
    void getSnakeInView(int rad);

   public:
    PolicyMaker(const char* _name) : view(5, 5) { strcpy(name, _name); }
    virtual ~PolicyMaker() {}

    void linkMap(Map* pmap) { theMap = pmap; }
    void linkFoodList(FoodList* pFL) { theFoods = pFL; }
    void linkpSnakes(Snake** pSs, int num) {
        pSnakes = pSs;
        snakeNum = num;
    }
    void setSnake(const Snake* pS) { pSnake = pS; }
    char* getName() { return name; }

    virtual Action actionToDo(int) = 0;
};

#ifndef NO_CURSES
class HumanAgent : public PolicyMaker {
   private:
    int U_key, D_key, L_key, R_key;
    int accelarate_key;

   public:
    HumanAgent(int Uk = KEY_UP,
               int Dk = KEY_DOWN,
               int Lk = KEY_LEFT,
               int Rk = KEY_RIGHT,
               int fk = ' ')
        : PolicyMaker("HumanAgent") {
        U_key = Uk;
        D_key = Dk;
        L_key = Lk;
        R_key = Rk;
        accelarate_key = fk;
    }
    Action actionToDo(int keyPressed) {
        if (keyPressed == U_key)
            return U_Act;
        else if (keyPressed == D_key)
            return D_Act;
        else if (keyPressed == L_key)
            return L_Act;
        else if (keyPressed == R_key)
            return R_Act;
        else if (keyPressed == accelarate_key)
            return ACC_Act;
        else
            return noAct;
    }
};
#else
#define HumanAgent RandomAgent
#endif

class RandomAgent : public PolicyMaker {
   public:
    RandomAgent() : PolicyMaker("RandomAgent") {}
    Action actionToDo(int arg) {
        // Avoid to hit its own bodys and walls
        // Random walk

        getView(1);

        std::vector<Action> candidates;

        // Check whether the wall is at the right side of its
        // head
        if (!view.isWall(5)) {
            candidates.push_back(R_Act);
        }
        // Check whether the wall is at the left side of its
        // head
        if (!view.isWall(3)) {
            candidates.push_back(L_Act);
        }
        // Check whether the wall is at the up side of its head
        if (!view.isWall(1)) {
            candidates.push_back(U_Act);
        }
        // Check whether the wall is at the down side of its
        // head
        if (!view.isWall(8)) {
            candidates.push_back(D_Act);
        }

        //Eliminate illegal actions
        for (int i = 0; i < candidates.size(); i++) {
            if ( isOppDir(candidates[i], pSnake->getLast())) {
                candidates.erase(candidates.begin() + i);
            }
        }

        //If _last action is in candidates, then chose to do _last action.
        for (int i = 0; i < candidates.size(); i++) {
            if (pSnake->getLast() == candidates[i]) {
                return pSnake->getLast();
            }
        }

        //Otherwise, randomly do an action
        if (candidates.size() ==0)
            candidates.push_back(pSnake->getNext());
        int r = rand() % candidates.size();
        return candidates[r];
    }
    bool isOppDir(Action a1, Action a2){
        if((a1 == U_Act && a2 == D_Act) ||
            (a1 == D_Act && a2 == U_Act) ||
            (a1 == R_Act && a2 == L_Act) ||
            (a1 == R_Act && a2 == R_Act))
            return true;
        return false;
    }

};

#endif  //__PolicyMaker_h__
