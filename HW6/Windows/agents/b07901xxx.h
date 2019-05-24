#ifndef __b07901xxx_h__
#define __b07901xxx_h__

//!! TODO 1: modify the ifndef/define protection as your ID like "__b07901xxx_h__"

#include "../PolicyMaker.h"

//!! TODO 2: rename your agent class name as "Agent_b07901xxx" with your own student ID
class Agent_b07901xxx:public PolicyMaker{
public:
    //!! TODO 3: put your student ID for the constructor of PolicyMaker (the base class)
    // you can have argument(s), but all these argument(s) must have their default value
    Agent_b07901xxx():PolicyMaker("b07901xxx"){}

    //{ ===== you can add any member functions and datas here =====
    //but don't use any static data!!!
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
    //} ===========================================================

//!! TODO 4: implement your own actionToDo function here
    virtual Action actionToDo(int arg){
        // view (radius = 1):
        // 00 01 02
        // 03 me 05
        // 06 07 08

        //view (radius = 2):
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
        
        return randMove();
    }
};

#endif //__b07901xxx_h__
