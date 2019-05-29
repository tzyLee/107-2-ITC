#ifndef __pyb06901145_h__
#define __pyb06901145_h__

//!! TODO 1: modify the ifndef/define protection as your ID like
//!"__pyb06901145_h__"

#include <fstream>
#include "../PolicyMaker.h"

//!! TODO 2: please change the file name to your ID
const char studentID[100] = "pyb06901145";
extern const char* pythonObjectID;
unsigned constexpr length(const char* str) {
    return *str ? 1 + length(str + 1) : 0;
}
constexpr unsigned len = length("python pyb06901145.py '");
//!! TODO 3: rename your agent class name as "Agent_pyb06901145" with your own
//! student ID
class Agent_pyb06901145 : public PolicyMaker {
   public:
    //!! TODO 4: put your student ID for the constructor of PolicyMaker (the
    //! base class)
    // you can have argument(s), but all these argument(s) must have their
    // default value

    Agent_pyb06901145() : PolicyMaker("pyb06901145") {}

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

    //} ===========================================================

    virtual Action actionToDo(int arg) {
        getView(3);
        // getSnakeInView(3);
        // getFoodInView(3);
        char command[300] = "python pyb06901145.py '";
        for (int i = 0; i < 24; ++i)
            command[len + i] = view[i];
        for (int i = 25; i < 49; ++i)
            command[len + i - 1] = view[i];
        command[len + 49 - 1] = '\'';
        command[len + 50 - 1] = ' ';
        command[len + 50] = '\0';
        strcat(command, pythonObjectID);

        char buffer[80] = "";
        FILE* fp = popen(command, "r");
        fgets(buffer, sizeof(buffer), fp);
        pclose(fp);

        return getMove((int)buffer[0] - 48);
    }
};

#endif  //__pythonAgent_h__
