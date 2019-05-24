#ifndef __b06901145_h__
#define __b06901145_h__

//!! TODO 1: modify the ifndef/define protection as your ID like
//!"__b06901145_h__"

#include <cassert>
#include <queue>
#include "../PolicyMaker.h"

//!! TODO 2: rename your agent class name as "Agent_b06901145" with your own
//! student ID
class Agent_b06901145 : public PolicyMaker {
   public:
    //!! TODO 3: put your student ID for the constructor of PolicyMaker (the
    //! base class)
    // you can have argument(s), but all these argument(s) must have their
    // default value
    Agent_b06901145() : PolicyMaker("b06901145") {}

    //{ ===== you can add any member functions and datas here =====
    // but don't use any static data!!!
    Action randMove() {
        int r = rand() % 6;
        // 2/6%: just go ahead
        // 4/6%: 1/6% for each dir
        switch (r) {
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
    //} ===========================================================

    //!! TODO 4: implement your own actionToDo function here
    virtual Action actionToDo(int arg) {
        // view (radius = 1):
        // 00 01 02
        // 03 me 05
        // 06 07 08

        // view (radius = 2):
        // 00 01 02 03 04
        // 05 06 07 08 09
        // 10 11 me 13 14
        // 15 16 17 18 19
        // 20 21 22 23 24
        // Max radius is 3

        // view (radius = 3):
        // 00 01 02 03 04 05 06
        // 07 08 09 10 11 12 13
        // 14 15 16 17 18 19 20
        // 21 22 23 me 25 26 27
        // 28 29 30 31 32 33 34
        // 35 36 37 38 39 40 41
        // 42 43 44 45 46 47 48

        bool choice[4] = {1, 1, 1, 1};  // U D L R
        getSnakeInView(3);
        getFoodInView(3);
        getView(3);
        for (auto& snake : SnakeinView) {
            for (auto& pos : snake._body) {
                std::cerr << pos << ' ';
            }
            std::cerr << '\n' << '\n';
        }

        if (view.isWall(23) || static_cast<char>(view[23] & 0xFF) == '*')
            choice[2] = false;

        if (view.isWall(31) || static_cast<char>(view[31] & 0xFF) == '*')
            choice[1] = false;

        if (view.isWall(25) || static_cast<char>(view[25] & 0xFF) == '*')
            choice[3] = false;

        if (view.isWall(17) || static_cast<char>(view[17] & 0xFF) == '*')
            choice[0] = false;

        int nearestFoodX = 3, nearestFoodY = 3;
        if (!FoodinView.empty()) {
            int pos = pSnake->getHeadPos();
            int h_x = pos % 120, h_y = pos / 40;
            auto comp = [&](const Food& a, const Food& b) {
                int a_dist_x = a.pos % 120 - h_x, a_dist_y = a.pos / 40 - h_y;
                int b_dist_x = b.pos % 120 - h_x, b_dist_y = b.pos / 40 - h_y;
                return a_dist_x * a_dist_x + a_dist_y * a_dist_y >
                       b_dist_x * b_dist_x + b_dist_y * b_dist_y;
            };
            std::priority_queue<Food, std::vector<Food>, decltype(comp)> pq(
                comp, FoodinView);

            nearestFoodX = 3 + (pq.top().pos % 120) - h_x;
            nearestFoodY = 3 + (pq.top().pos / 40) - h_y;
        }
        if (nearestFoodY < 3 && choice[0])
            return Action::U_Act;
        if (nearestFoodY > 3 && choice[1])
            return Action::D_Act;
        if (nearestFoodX < 3 && choice[2])
            return Action::L_Act;
        if (nearestFoodX > 3 && choice[3])
            return Action::R_Act;

        if (choice[0])
            return Action::U_Act;
        if (choice[1])
            return Action::D_Act;
        if (choice[2])
            return Action::L_Act;
        if (choice[3])
            return Action::R_Act;
        return Action::R_Act;
    }
};

#endif  //__b06901145_h__
