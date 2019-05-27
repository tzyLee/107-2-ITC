#ifndef __b06901145_h__
#define __b06901145_h__

//!! TODO 1: modify the ifndef/define protection as your ID like
//!"__b06901145_h__"

#include <algorithm>
#include <cassert>
#include <cmath>
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
        // Avoid wall and snake
        if (dangerous(23))
            choice[2] = false;
        if (dangerous(31))
            choice[1] = false;
        if (dangerous(25))
            choice[3] = false;
        if (dangerous(17))
            choice[0] = false;
        // Dead loop prevention (Only for nearby 7*7)
        if (choice[0] && !safe(&view[0], 3, 2))
            choice[0] = false;
        if (choice[1] && !safe(&view[0], 3, 4))
            choice[1] = false;
        if (choice[2] && !safe(&view[0], 2, 3))
            choice[2] = false;
        if (choice[3] && !safe(&view[0], 4, 3))
            choice[3] = false;
        // Eat nearest food
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
            if (nearestFoodY < 3 && choice[0])
                return Action::U_Act;
            if (nearestFoodY > 3 && choice[1])
                return Action::D_Act;
            if (nearestFoodX < 3 && choice[2])
                return Action::L_Act;
            if (nearestFoodX > 3 && choice[3])
                return Action::R_Act;
        }
        // Early return the only possible move, preventing the rating being
        // subtracted to zero
        if (choice[0] && !choice[1] && !choice[2] && !choice[3])
            return Action::U_Act;
        if (!choice[0] && choice[1] && !choice[2] && !choice[3])
            return Action::D_Act;
        if (!choice[0] && !choice[1] && choice[2] && !choice[3])
            return Action::L_Act;
        if (!choice[0] && !choice[1] && !choice[2] && choice[3])
            return Action::R_Act;
        // Rate choice based on number of safe blocks
        unsigned rating[4] = {0, 0, 0, 0};
        if (choice[0])
            rating[0] = rate(Action::U_Act);
        if (choice[1])
            rating[1] = rate(Action::D_Act);
        if (choice[2])
            rating[2] = rate(Action::L_Act);
        if (choice[3])
            rating[3] = rate(Action::R_Act);
        // Move toward body-less area
        int headX = pSnake->getHeadPos() % 120,
            headY = pSnake->getHeadPos() / 120;
        const int bodyBias = 1;
        for (int i : pSnake->_body) {
            int x = i % 120, y = i / 120;
            if (x > headX)  // to the right of head
                rating[3] = rating[3] > bodyBias ? rating[3] - bodyBias : 0;
            else if (x < headX)  // to the left of head
                rating[2] = rating[2] > bodyBias ? rating[2] - bodyBias : 0;
            if (y > headY)  // to the bottom of head
                rating[1] = rating[1] > bodyBias ? rating[1] - bodyBias : 0;
            else if (y < headY)  // to the top of head
                rating[0] = rating[0] > bodyBias ? rating[0] - bodyBias : 0;
        }
        for (const Snake& snake : SnakeinView) {
            for (int i : snake._body) {
                int x = i % 120, y = i / 120;
                if (x > headX)  // to the right of head
                    rating[3] = rating[3] > bodyBias ? rating[3] - bodyBias : 0;
                else if (x < headX)  // to the left of head
                    rating[2] = rating[2] > bodyBias ? rating[2] - bodyBias : 0;
                if (y > headY)  // to the bottom of head
                    rating[1] = rating[1] > bodyBias ? rating[1] - bodyBias : 0;
                else if (y < headY)  // to the top of head
                    rating[0] = rating[0] > bodyBias ? rating[0] - bodyBias : 0;
            }
        }
        const int headBias = 10;
        // Player head detection
        if (view[16] == '@') {
            rating[0] = rating[0] > headBias ? rating[0] - headBias : 0;
            rating[2] = rating[2] > headBias ? rating[2] - headBias : 0;
        }
        if (view[18] == '@') {
            rating[0] = rating[0] > headBias ? rating[0] - headBias : 0;
            rating[3] = rating[3] > headBias ? rating[3] - headBias : 0;
        }
        if (view[30] == '@') {
            rating[1] = rating[1] > headBias ? rating[1] - headBias : 0;
            rating[2] = rating[2] > headBias ? rating[2] - headBias : 0;
        }
        if (view[32] == '@') {
            rating[1] = rating[1] > headBias ? rating[1] - headBias : 0;
            rating[3] = rating[3] > headBias ? rating[3] - headBias : 0;
        }
        int sum = rating[0] + rating[1] + rating[2] + rating[3];
        if (sum) {
            unsigned random = rand() % sum, lowerBound = 0;
            // Choose available action
            if (choice[0] && random < rating[0])
                return Action::U_Act;
            lowerBound += rating[0];
            if (choice[1] && lowerBound <= random &&
                random < lowerBound + rating[1])
                return Action::D_Act;
            lowerBound += rating[1];
            if (choice[2] && lowerBound <= random &&
                random < lowerBound + rating[2])
                return Action::L_Act;
            lowerBound += rating[2];
            if (choice[3] && lowerBound <= random)
                return Action::R_Act;
        } else {  // The sum may be all zero, but some choice are unsafe
            int numOfPossible = choice[0] + choice[1] + choice[2] + choice[3];
            unsigned random = (rand() % numOfPossible) + 1;
            int actionIndex = 0;
            for (int i = 0; i < 4; ++i)
                if (choice[i]) {
                    ++actionIndex;
                    if (random == actionIndex)
                        break;
                }
            if (actionIndex == 1)
                return Action::U_Act;
            if (actionIndex == 2)
                return Action::D_Act;
            if (actionIndex == 3)
                return Action::L_Act;
            if (actionIndex == 4)
                return Action::R_Act;
        }
        return Action::R_Act;
    }

    bool dangerous(int pos) {
        char block = view[pos] & 0xFF;
        return view.isWall(pos) || block == '*' || block == '@';
    }

    bool dangerous(chtype map[49], int pos) {
        char block = map[pos] & 0xFF;
        return block == '#' || block == '*' || block == '@';
    }

    bool safe(chtype map[49], int x, int y) {
        if (dangerous(map, 7 * y + x))
            return false;
        if (x == 0 || x == 7 || y == 0 || y == 7)
            return true;
        chtype temp = map[7 * y + x];
        map[7 * y + x] = '#';
        if (safe(map, x + 1, y) || safe(map, x - 1, y) || safe(map, x, y + 1) ||
            safe(map, x, y - 1)) {
            map[7 * y + x] = temp;
            return true;
        }
        map[7 * y + x] = temp;
        return false;
    }

    unsigned rate(Action action) {
        int count = 0;
        switch (action) {
            case Action::U_Act:
                for (int i = 0; i < 21; ++i)
                    if (!dangerous(i))
                        ++count;
                return count;
            case Action::D_Act:
                for (int i = 28; i < 49; ++i)
                    if (!dangerous(i))
                        ++count;
                return count;
            case Action::L_Act:
                for (int i = 0; i < 7; ++i)
                    for (int j = 0; j < 3; ++j)
                        if (!dangerous(7 * i + j))
                            ++count;
                return count;
            case Action::R_Act:
                for (int i = 0; i < 7; ++i)
                    for (int j = 4; j < 7; ++j)
                        if (!dangerous(7 * i + j))
                            ++count;
                return count;

            default:
                return 0;
        }
    }
};

#endif  //__b06901145_h__