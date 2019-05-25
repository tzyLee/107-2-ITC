#ifndef __b07901000_h__
#define __b07901000_h__

// the same as default b05901xxx

#include "../PolicyMaker.h"

class Agent_b07901000:public PolicyMaker{
public:
    Agent_b07901000():PolicyMaker("b07901000"){}
    Action randMove(){
        int r = rand()%6;
        //2/6%: just go ahead
        //4/6%: 1/6% for each dir
        switch(r){
        case 0:
        case 1:
        case 2:
            return U_Act;
        case 3:
            return D_Act;
        case 4:
            return L_Act;
        case 5:
            return R_Act;
        default:
            return noAct;
        }
    }
    virtual Action actionToDo(int arg){
        // Avoid to hit its own bodys and walls
        // Random walk

        getView(1);

        std::vector<Action> candidates;

        int headX, headY;
        pos2xy(pSnake->getHeadPos(), headX, headY);
        // Check whether the snake's body or the wall at the right side of its
        // head
        if (!atBody(pSnake, headX + 1, headY) && !view.isWall(5)) {
            candidates.push_back(R_Act);
        }
        // Check whether the snake's body or the wall at the left side of its
        // head
        if (!atBody(pSnake, headX - 1, headY) && !view.isWall(3)) {
            candidates.push_back(L_Act);
        }
        // Check whether the snake's body or the wall at the up side of its head
        if (!atBody(pSnake, headX, headY + 1) && !view.isWall(1)) {
            candidates.push_back(U_Act);
        }
        // Check whether the snake's body or the wall at the down side of its
        // head
        if (!atBody(pSnake, headX, headY - 1) && !view.isWall(8)) {
            candidates.push_back(D_Act);
        }

        // Eliminate illegal actions
        for (int i = 0; i < candidates.size(); i++) {
            if (isOppDir(candidates[i], pSnake->getLast())) {
                candidates.erase(candidates.begin() + i);
            }
        }

        //If a food is nearby, eat it
        for (int i = 0; i < candidates.size(); i++) {
            if(view.isFood(candidates[i]))
                return candidates[i];
        }

        // Otherwise, randomly do an action
        if (candidates.size() == 0)
            candidates.push_back(pSnake->getNext());

        int r = rand() % candidates.size();
        return candidates[r];
    }
    bool atBody(const Snake* target, int x, int y) {
        for (int i = 1; i < target->getBody().size(); i++) {
            int targetX, targetY;
            pos2xy(target->getBody()[i], targetX, targetY);
            if ((targetX == x) && (targetY == y))
                return true;
        }
        return false;
    }
    bool isOppDir(Action a1, Action a2) {
        if ((a1 == U_Act && a2 == D_Act) || (a1 == D_Act && a2 == U_Act) ||
            (a1 == R_Act && a2 == L_Act) || (a1 == R_Act && a2 == R_Act))
            return true;
        return false;
    }
};

#endif //__b07901000_h__
