#include "basic.h"

#include <string>

std::string DebuggingMessage;

#ifndef NO_CURSES

#else
    #include <cstdio>
    #include <cstdarg>
    #include <cstring>

    char buffer[1024];

    int printw(const char *fmt, ...){
        char input[256];
        va_list vl;
        va_start(vl, fmt);
        int i = vsprintf(input, fmt, vl);
        va_end(vl);

        strcat(buffer,input);

        return i;
    }

    void mvaddch(int i,int j,chtype c){
        printw("%c",c);
    }

    void refresh(){
        printf("%s",buffer);
        buffer[0] = '\0';
    }

#endif





