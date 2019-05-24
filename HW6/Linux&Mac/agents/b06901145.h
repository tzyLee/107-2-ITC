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
   private:
    unsigned _starvation, _lastBodyLength;

   public:
    //!! TODO 3: put your student ID for the constructor of PolicyMaker (the
    //! base class)
    // you can have argument(s), but all these argument(s) must have their
    // default value
    Agent_b06901145()
        : PolicyMaker("b06901145"), _starvation(0), _lastBodyLength(0) {}

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
        if (_lastBodyLength != pSnake->_finalBodyLength)
            ++_starvation;

        bool choice[4] = {1, 1, 1, 1};  // U D L R
        getSnakeInView(3);
        getFoodInView(3);
        getView(3);
        // for (auto& snake : SnakeinView) {
        //     for (auto& pos : snake._body) {
        //         std::cerr << pos << ' ';
        //     }
        //     std::cerr << '\n' << '\n';
        // }

        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                std::cerr << static_cast<char>(view[7 * i + j]);
            }
            std::cerr << '\n';
        }
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
        if (!safe(&view[0], 3, 2))
            choice[0] = false;
        if (!safe(&view[0], 3, 4))
            choice[1] = false;
        if (!safe(&view[0], 2, 3))
            choice[2] = false;
        if (!safe(&view[0], 4, 3))
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
        }
        if (nearestFoodY < 3 && choice[0])
            return Action::U_Act;
        if (nearestFoodY > 3 && choice[1])
            return Action::D_Act;
        if (nearestFoodX < 3 && choice[2])
            return Action::L_Act;
        if (nearestFoodX > 3 && choice[3])
            return Action::R_Act;
        // Starvation, prioritize right and down
        if (_starvation > 10) {
            if (choice[3]) {
                _starvation -= 5;
                return Action::R_Act;
            } else if (choice[1]) {
                _starvation -= 5;
                return Action::D_Act;
            }
        }
        int random = rand() % 100 < 80;
        // Choose available action
        if (choice[0] && random)
            return Action::U_Act;
        if (choice[1])
            return Action::D_Act;
        if (choice[2] && random)
            return Action::L_Act;
        if (choice[3])
            return Action::R_Act;
        if (choice[0])
            return Action::U_Act;
        if (choice[2])
            return Action::L_Act;
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
    // bool safe(chtype map[49], Action action, int depth = 0) {
    //     if (map[17] == 'x' || map[23] == 'x' || map[31] == 'x' ||
    //         map[25] == 'x')
    //         // base case, check until the block is unknown
    //         return true;
    //     if (depth > 3)
    //         return true;
    //     chtype newMap[49];
    //     map[24] = '*';
    //     switch (action) {
    //         case Action::U_Act:
    //             std::copy(&map[0], &map[42], &newMap[7]);
    //             std::fill(&newMap[0], &newMap[7], 'x');
    //             break;
    //         case Action::D_Act:
    //             std::copy(&map[7], &map[49], &newMap[0]);
    //             std::fill(&newMap[42], &newMap[49], 'x');
    //             break;
    //         case Action::L_Act:
    //             for (int i = 0; i < 7; ++i) {
    //                 std::copy(&map[7 * i], &map[7 * i + 6], &newMap[7 * i +
    //                 1]); newMap[7 * i] = 'x';
    //             }
    //             break;
    //         case Action::R_Act:
    //             for (int i = 0; i < 7; ++i) {
    //                 std::copy(&map[7 * i + 1], &map[7 * (i + 1)],
    //                           &newMap[7 * i]);
    //                 newMap[7 * i + 6] = 'x';
    //             }
    //             break;
    //         default:
    //             map[24] = '@';
    //             return false;
    //     }
    //     map[24] = newMap[24] = '@';
    //     // Dead loop prevention
    //     if (dangerous(map, 10) && dangerous(map, 16) && dangerous(map, 18))
    //         return false;
    //     if (dangerous(map, 30) && dangerous(map, 32) && dangerous(map, 38))
    //         return false;
    //     if (dangerous(map, 16) && dangerous(map, 22) && dangerous(map, 30))
    //         return false;
    //     if (dangerous(map, 18) && dangerous(map, 26) && dangerous(map, 32))
    //         return false;
    //     bool possible[4] = {
    //         action != Action::D_Act && safe(newMap, Action::U_Act, depth +
    //         1), action != Action::U_Act && safe(newMap, Action::D_Act, depth
    //         + 1), action != Action::R_Act && safe(newMap, Action::L_Act,
    //         depth + 1), action != Action::L_Act && safe(newMap,
    //         Action::R_Act, depth + 1)};
    //     switch (action) {
    //         case Action::U_Act:
    //             return possible[1] || possible[2] || possible[3];
    //             break;
    //         case Action::D_Act:
    //             return possible[0] || possible[2] || possible[3];
    //             break;
    //         case Action::L_Act:
    //             return possible[0] || possible[1] || possible[3];
    //             break;
    //         case Action::R_Act:
    //             return possible[0] || possible[1] || possible[2];
    //             break;
    //         default:
    //             return false;
    //     }
    // }
};

#endif  //__b06901145_h__
