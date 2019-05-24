#ifndef __AgentsMgr_h__
#define __AgentsMgr_h__

#include <cstdio>
#include <string>
#include <vector>
//!! TODO 1: put your h/cpp files in "agents" folder
//!! TODO 2: include your b06901145.h file here
#include "agents/b06901145.h"
#include "agents/pyb06901145.h"

// function pionter
typedef PolicyMaker* (*pfNewAgent)(void);

template <class T>
PolicyMaker* fNewAgent() {
    return new T;
}

class AgentsMgr {
   public:
    std::vector<pfNewAgent> pAllNewAgentFunc;
    std::vector<std::string> agentName;
    int** scores;

    AgentsMgr() {
        //!! TODO 3: add your agent class "Agent_b05901xxx" in a new push_back,
        //!so TA can "new" your agent
        // number of snakes should be 4 !!!

        pAllNewAgentFunc.push_back(
            &fNewAgent<Agent_b06901145>);  // for c++ agent
        // pAllNewAgentFunc.push_back(&fNewAgent<Agent_pyb06901145>);// for
        // python agent

        pAllNewAgentFunc.push_back(&fNewAgent<RandomAgent>);
        pAllNewAgentFunc.push_back(&fNewAgent<RandomAgent>);
        pAllNewAgentFunc.push_back(&fNewAgent<RandomAgent>);

        int size = pAllNewAgentFunc.size();
        scores = new int*[size];
        for (int i = 0; i < size; ++i)
            scores[i] = new int[size];
    }
    ~AgentsMgr() {
        int size = pAllNewAgentFunc.size();
        for (int i = 0; i < size; ++i)
            delete[] scores[i];
        delete[] scores;
    }
    void dump(const char* fileName) {
        FILE* pFile;
        int size = pAllNewAgentFunc.size();

        pFile = fopen(fileName, "w");

        fprintf(pFile, "0 ,");
        for (int i = 0; i < size; ++i) {
            fprintf(pFile, "%s ,", agentName[i].c_str());
        }
        fprintf(pFile, "\n");

        for (int i = 0; i < size; ++i) {
            fprintf(pFile, "%s ,", agentName[i].c_str());
            for (int j = 0; j < size; ++j) {
                // if(i==j)
                fprintf(pFile, "%d ,", scores[i][j]);
            }
            fprintf(pFile, "\n");
        }
        fclose(pFile);
    }
};

#endif  //__AgentsMgr_h__