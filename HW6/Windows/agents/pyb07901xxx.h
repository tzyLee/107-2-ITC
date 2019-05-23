#ifndef __pyb07901xxx_h__
#define __pyb07901xxx_h__

//!! TODO 1: modify the ifndef/define protection as your ID like "__pyb07901xxx_h__"

#include "../PolicyMaker.h"
#include <fstream>


//!! TODO 2: please change the file name to your ID
const char studentID[100]="pyb07901xxx";

//!! TODO 3: rename your agent class name as "Agent_pyb07901xxx" with your own student ID
class Agent_pyb07901xxx:public PolicyMaker{
public:
//!! TODO 4: put your student ID for the constructor of PolicyMaker (the base class)
// you can have argument(s), but all these argument(s) must have their default value

    Agent_pyb07901xxx():PolicyMaker("pyb07901xxx"){}

    Action getMove(int r){
        //2/6%: just go ahead
        //4/6%: 1/6% for each dir
        switch(r){
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

     void strToFile(string input,char * file){
   
         std::ofstream out(file);
         out << input;
         out.close();

     };



    //} ===========================================================

    virtual Action actionToDo(int arg){

	getView(3);
        getSnakeInView(3);
        getFoodInView(3);

        string viewStr=view.getMapStr();

        char file[100];
        strcat(file,"agents/");
        strcat(file,studentID);
        strcat(file,".txt");

        char command[100]="python agents/";
        strcat(command,studentID);
        strcat(command,".py  agents/");
        strcat(command,studentID);
        strcat(command,".py");
        strToFile(viewStr,file);
        FILE * fp;
    	char buffer[80]="";
    	fp = popen(command, "r");
    	fgets(buffer, sizeof(buffer), fp);
    	pclose(fp);

        return getMove( (int)buffer[0]-48);

    }

   
};

#endif //__pythonAgent_h__
