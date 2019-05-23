#include "PolicyMaker.h"

void PolicyMaker::getView(int rad){
    view.clear();
    view.copy_local(*theMap, pSnake->getHeadPos(), rad > 3 ? 3 : rad);
}

void PolicyMaker::getFoodInView(int rad){
    rad = (rad>3)?3:rad;
    FoodinView.clear();
    int Tx,Ty,Mx,My,dx,dy;
    pos2xy(pSnake->getHeadPos(),Tx,Ty);

    FoodList::iterator it = theFoods->begin();
    while (it != theFoods->end()) {
        pos2xy(it->getPos(),Mx,My);
        dx = Tx-Mx; dx=(dx<0)?-dx:dx;
        dy = Ty-My; dy=(dy<0)?-dy:dy;
        if(dx<=rad && dy<=rad)
            FoodinView.push_back(*it);
        ++it;
    }
}

void PolicyMaker::getSnakeInView(int rad){
    rad = (rad>3)?3:rad;
    SnakeinView.clear();
    int Tx,Ty,T2x,T2y,dx,dy;
    pos2xy(pSnake->getHeadPos(),Tx,Ty);

    for(int i=0;i<snakeNum;++i){
        if(pSnakes[i]->getHeadPos()==pSnake->getHeadPos()) continue;
        pos2xy(pSnakes[i]->getHeadPos(),T2x,T2y);
        dx = Tx-T2x; dx=(dx<0)?-dx:dx;
        dy = Ty-T2y; dy=(dy<0)?-dy:dy;
        if(dx<=rad && dy<=rad)
            SnakeinView.push_back(*pSnakes[i]);
    }
}



