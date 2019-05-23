#ifndef __basic_h__
#define __basic_h__

// We advise you to build up curses library.
// You can use either PDCurses or NCurses (both are cross-platform)
// If you really don't want to use them or have big trouble on that,
// you can uncomment the following line to build LittleTank without curses.
// But in so doing, you can't enjoy keyboard agent or colors anymore.

//#define NO_CURSES

#ifndef NO_CURSES
#include "curses.h"
#else
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>


#define COLOR_WHITE 0
#define COLOR_GREEN 0
#define COLOR_YELLOW 1
#define COLOR_RED 2
typedef char chtype;

using namespace std;

int printw(const char* fmt, ...);
void mvaddch(int, int, chtype c);
void refresh();

inline void napms(int num) {
    clock_t t = clock();
    while (clock() - t < num) {
    }
}
#endif

enum Action { noAct = 0, U_Act = 1, D_Act = 2, L_Act = 3, R_Act = 4, ACC_Act = 5 };

#endif  // __basic_h__
