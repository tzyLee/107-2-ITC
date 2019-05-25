#ifndef __pyb06901145_h__
#define __pyb06901145_h__

//!! TODO 1: modify the ifndef/define protection as your ID like
//!"__pyb06901145_h__"

#include <fstream>
#include "../PolicyMaker.h"

//!! TODO 2: please change the file name to your ID
const char studentID[100] = "pyb06901145";
//!! TODO 3: rename your agent class name as "Agent_pyb06901145" with your own
//! student ID
class Agent_pyb06901145 : public PolicyMaker {
   public:
    //!! TODO 4: put your student ID for the constructor of PolicyMaker (the
    //! base class)
    // you can have argument(s), but all these argument(s) must have their
    // default value

    Agent_pyb06901145() : PolicyMaker("pyb06901145") {}

    ~Agent_pyb06901145() {}

    Action getMove(int r) {
        // 2/6%: just go ahead
        // 4/6%: 1/6% for each dir
        switch (r) {
            case 0:
                return noAct;
            case 1:
                return U_Act;
            case 2:
                return D_Act;
            case 3:
                return L_Act;
            case 4:
                return R_Act;
            case 5:
                return ACC_Act;
            default:
                return noAct;
        }
    }

    void strToFile(string input, char* file) {
        std::ofstream out(file);
        out << input;
        out.close();
    };

    //} ===========================================================

    virtual Action actionToDo(int arg) {
        getView(3);
        getSnakeInView(3);
        getFoodInView(3);
        int headPos = pSnake->getHeadPos();
        int upperLeft = headPos - 120 * 3 - 3;
        std::string mapString(120 * 40, '?');
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                int index = upperLeft + 120 * i + j;
                if (index >= 0 &&
                    index < 120 * 40)  // prevent from 'x' segfault
                    mapString[upperLeft + 120 * i + j] = view[i * 7 + j];
            }
        }
        for (int i = 0; i < 120; ++i)
            mapString[i] = '#';
        for (int i = 0; i < 40; ++i)
            mapString[120 * i] = '#';
        for (int i = 0; i < 120; ++i)
            mapString[39 * 120 + i] = '#';
        for (int i = 0; i < 40; ++i)
            mapString[120 * i + 119] = '#';
        for (auto& snake : SnakeinView) {
            for (auto& pos : snake._body) {
                mapString[pos] = '*';
            }
            mapString[snake._body[0]] = '@';
        }
        mapString[headPos] = '+';  // Head of player itself
        // for (int i = 0; i < 40; ++i) {
        //     for (int j = 0; j < 120; ++j)
        //         std::cout << mapString[120 * i + j];
        //     std::cout << '\n';
        // }
        std::cout << mapString << std::endl;
        std::cout.flush();
        int res = 0;
        std::cin >> res;
        return getMove(res);
    }
};

#endif  //__pythonAgent_h__
