#include "basic.h"

#include <string>

std::string DebuggingMessage;

#ifdef NO_CURSES
#include <cstdarg>
#include <cstdio>
#include <cstring>

char buffer[1024];

int printw(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    int i = vprintf(fmt, vl);
    va_end(vl);
    return i;
}

void mvaddch(int i, int j, chtype c) { /*printw("%c", c);*/
}

void refresh() { /*
     printf("%s", buffer);
     buffer[0] = '\0';*/
}

#endif
