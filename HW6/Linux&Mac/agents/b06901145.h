#ifndef __b06901145_h__
#define __b06901145_h__

//!! TODO 1: modify the ifndef/define protection as your ID like
//!"__b06901145_h__"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <queue>
#include <vector>
#include "../PolicyMaker.h"

//!! TODO 2: rename your agent class name as "Agent_b06901145" with your own
//! student ID
class Agent_b06901145 : public PolicyMaker {
    std::bitset<40 * 120> map;  // false: safe, true: unsafe

   public:
    //!! TODO 3: put your student ID for the constructor of PolicyMaker (the
    //! base class)
    // you can have argument(s), but all these argument(s) must have their
    // default value
    Agent_b06901145() : PolicyMaker("b06901145"), map() {}

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
        if (dangerous(17))  // Up
            choice[0] = false;
        if (dangerous(31))  // Down
            choice[1] = false;
        if (dangerous(23))  // Left
            choice[2] = false;
        if (dangerous(25))  // Right
            choice[3] = false;
        // Dead loop prevention (Only for nearby 7*7)
        if (choice[0] && !safe(&view[0], 3, 2))  // Up
            choice[0] = false;
        if (choice[1] && !safe(&view[0], 3, 4))  // Down
            choice[1] = false;
        if (choice[2] && !safe(&view[0], 2, 3))  // Left
            choice[2] = false;
        if (choice[3] && !safe(&view[0], 4, 3))  // Right
            choice[3] = false;
        // TODO Dead loop prevention (full)
        map.reset();  // set all bits to false (safe)
        int pos = pSnake->getHeadPos();
        int headX = pos % 120, headY = pos / 120;
        // Get Body Info
        unsigned bodyCount[4] = {0, 0, 0, 0};  // U D L R
        std::vector<int> body(std::move(pSnake->getBody()));
        for (int i : pSnake->_body) {
            int x = i % 120, y = i / 120;
            map[i] = true;
            if (x > headX)  // to the right of head
                ++bodyCount[3];
            else if (x < headX)  // to the left of head
                ++bodyCount[2];
            if (y > headY)  // to the bottom of head
                ++bodyCount[1];
            else if (y < headY)  // to the top of head
                ++bodyCount[0];
        }
        for (const Snake& snake : SnakeinView) {
            for (int i : snake._body) {
                int x = i % 120, y = i / 120;
                map[i] = true;
                if (x > headX)  // to the right of head
                    ++bodyCount[3];
                else if (x < headX)  // to the left of head
                    ++bodyCount[2];
                if (y > headY)  // to the bottom of head
                    ++bodyCount[1];
                else if (y < headY)  // to the top of head
                    ++bodyCount[0];
            }
        }
        // Set wall to true
        for (int i = 0; i < 120; ++i) {
            map[i] = true;
            map[120 * 39 + i] = true;
        }
        for (int i = 0; i < 40; ++i) {
            map[120 * i] = true;
            map[120 * (i + 1) - 1] = true;
        }
        if (choice[0] && !noLoop(headX, headY, Action::U_Act, body))
            choice[0] = false;
        if (choice[1] && !noLoop(headX, headY, Action::D_Act, body))
            choice[1] = false;
        if (choice[2] && !noLoop(headX, headY, Action::L_Act, body))
            choice[2] = false;
        if (choice[3] && !noLoop(headX, headY, Action::R_Act, body))
            choice[3] = false;
        // Eat nearest food
        if (!FoodinView.empty()) {
            int nearestFoodX = 3, nearestFoodY = 3;
            auto comp = [&](const Food& a, const Food& b) {
                int a_dist_x = a.pos % 120 - headX,
                    a_dist_y = a.pos / 120 - headY;
                int b_dist_x = b.pos % 120 - headX,
                    b_dist_y = b.pos / 120 - headY;
                return a_dist_x * a_dist_x + a_dist_y * a_dist_y >
                       b_dist_x * b_dist_x + b_dist_y * b_dist_y;
            };
            std::priority_queue<Food, std::vector<Food>, decltype(comp)> pq(
                comp, FoodinView);

            nearestFoodX = 3 + (pq.top().pos % 120) - headX;
            nearestFoodY = 3 + (pq.top().pos / 120) - headY;
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
        const double bodyBias = 0.82;
        unsigned temp = 0;  // prevent unsigned integer underflow
        temp = static_cast<unsigned>(bodyBias * bodyCount[0]);
        rating[0] = rating[0] > temp ? rating[0] - temp : 0;
        temp = static_cast<unsigned>(bodyBias * bodyCount[1]);
        rating[1] = rating[1] > temp ? rating[1] - temp : 0;
        temp = static_cast<unsigned>(bodyBias * bodyCount[2]);
        rating[2] = rating[2] > temp ? rating[2] - temp : 0;
        temp = static_cast<unsigned>(bodyBias * bodyCount[3]);
        rating[3] = rating[3] > temp ? rating[3] - temp : 0;
        const int headBias = 8;
        // Player head detection
        // TODO 10, 22, 26, 38的蛇的頭的位置
        if (view[16] & 0xFF == '@') {
            rating[0] = rating[0] > headBias ? rating[0] - headBias : 0;
            rating[2] = rating[2] > headBias ? rating[2] - headBias : 0;
        }
        if (view[18] & 0xFF == '@') {
            rating[0] = rating[0] > headBias ? rating[0] - headBias : 0;
            rating[3] = rating[3] > headBias ? rating[3] - headBias : 0;
        }
        if (view[30] & 0xFF == '@') {
            rating[1] = rating[1] > headBias ? rating[1] - headBias : 0;
            rating[2] = rating[2] > headBias ? rating[2] - headBias : 0;
        }
        if (view[32] & 0xFF == '@') {
            rating[1] = rating[1] > headBias ? rating[1] - headBias : 0;
            rating[3] = rating[3] > headBias ? rating[3] - headBias : 0;
        }
        // Boost rating of certain direction when length is too small
        // if (pSnake->_finalBodyLength < 6) {
        //     for (int i = 3; i >= 0; --i)
        //         if (choice[i]) {
        //             rating[i] += 30;
        //             break;
        //         }
        // }
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
        }
        // The sum may be all zero, but some choice are unsafe
        int numOfPossible = choice[0] + choice[1] + choice[2] + choice[3];
        if (numOfPossible) {
            unsigned random = (rand() % numOfPossible) + 1;
            int actionIndex = 0, ind = 0;
            for (; ind < 4; ++ind)
                if (choice[ind]) {
                    ++actionIndex;
                    if (random == actionIndex)
                        break;
                }
            if (random == actionIndex) {
                if (ind == 0)
                    return Action::U_Act;
                if (ind == 1)
                    return Action::D_Act;
                if (ind == 2)
                    return Action::L_Act;
                if (ind == 3)
                    return Action::R_Act;
            }
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
        if (x == 0 || x == 6 || y == 0 || y == 6)
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

    bool noLoop(int headX, int headY, Action action, std::vector<int>& body) {
        int newX = headX, newY = headY;
        std::bitset<40 * 120> temp = map;
        int area[2] = {0, 0};  // L R or U D
        switch (action) {
            case Action::U_Act:
                --newY;
                if (body[1] % 120 != newX)  // Not three in a column
                    return true;
                area[0] = countByFlooding(newX - 1, newY);
                map = temp;
                area[1] = countByFlooding(newX + 1, newY);
                map = temp;
                break;
            case Action::D_Act:
                ++newY;
                if (body[1] % 120 != newX)  // Not three in a column
                    return true;
                area[0] = countByFlooding(newX - 1, newY);
                map = temp;
                area[1] = countByFlooding(newX + 1, newY);
                map = temp;
                break;
            case Action::L_Act:
                --newX;
                if (body[1] / 120 != newY)  // Not three in a row
                    return true;
                area[0] = countByFlooding(newX, newY - 1);
                map = temp;
                area[1] = countByFlooding(newX, newY + 1);
                map = temp;
                break;
            case Action::R_Act:
                ++newX;
                if (body[1] / 120 != newY)  // Not three in a row
                    return true;
                area[0] = countByFlooding(newX, newY - 1);
                map = temp;
                area[1] = countByFlooding(newX, newY + 1);
                map = temp;
                break;
            default:
                break;
        }
        if (area[0] == area[1])
            return true;
        if (area[0] == 0) {
            Action direction =
                action == Action::U_Act || action == Action::D_Act
                    ? Action::R_Act
                    : Action::D_Act;
            return canMove(newX, newY, direction, body, area[1]);
        } else if (area[1] == 0) {
            Action direction =
                action == Action::U_Act || action == Action::D_Act
                    ? Action::L_Act
                    : Action::U_Act;
            return canMove(newX, newY, direction, body, area[0]);
        } else {
            Action direction =
                area[0] < area[1]
                    ? (action == Action::U_Act || action == Action::D_Act
                           ? Action::R_Act
                           : Action::D_Act)
                    : (action == Action::U_Act || action == Action::D_Act
                           ? Action::L_Act
                           : Action::U_Act);
            return canMove(newX, newY, direction, body,
                           std::min(area[0], area[1]));
        }
    }

    int countByFlooding(int x, int y) {
        if (x == 0 || x == 119 || y == 0 || y == 39)  // out of bound
            return 0;
        int area = 0;
        if (!map[120 * y + x]) {
            map[120 * y + x] = true;
            ++area;
        }
        if (!map[120 * y + x + 1])
            area += countByFlooding(x + 1, y);
        if (!map[120 * y + x - 1])
            area += countByFlooding(x - 1, y);
        if (!map[120 * (y + 1) + x])
            area += countByFlooding(x, y + 1);
        if (!map[120 * (y - 1) + x])
            area += countByFlooding(x, y - 1);
        return area;
    }

    bool canMove(int headX, int headY, Action action, std::vector<int>& body,
                 int limit) {
        if (limit <= 0)
            return true;
        int newX = headX, newY = headY;
        switch (action) {
            case Action::U_Act:
                --newY;
                break;
            case Action::D_Act:
                ++newY;
                break;
            case Action::L_Act:
                --newX;
                break;
            case Action::R_Act:
                ++newX;
                break;
            default:
                break;
        }
        if (newX == 0 || newX == 119 || newY == 0 ||
            newY == 39)  // out of bound
            return false;
        int newPos = 120 * newY + newX;
        int prevTailPos = body.back();
        map[newPos] = true;  // move to newPos, set to dangerous (head)
        map[prevTailPos] = false;
        body.pop_back();
        body.insert(body.cbegin(), newPos);
        // Check all direction(Recursion)
        bool isSafe = map[newPos - 120] &&
                          canMove(newX, newY, Action::U_Act, body, limit - 1) ||
                      map[newPos + 120] &&
                          canMove(newX, newY, Action::D_Act, body, limit - 1) ||
                      map[newPos - 1] &&
                          canMove(newX, newY, Action::L_Act, body, limit - 1) ||
                      map[newPos + 1] &&
                          canMove(newX, newY, Action::R_Act, body, limit - 1);
        // Restore head position, add tail back
        map[newPos] = false;
        map[prevTailPos] = true;
        body.erase(body.cbegin());
        body.push_back(prevTailPos);
        return isSafe;
    }
};

#endif  //__b06901145_h__
