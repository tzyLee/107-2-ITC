#ifndef __FOOD_H__
#define __FOOD_H__

#include <list>
#include "basic.h"

class Food {
   public:
    int pos;
    Food(int p) { pos = p; }

    void update(Map& map) {
        if (map[pos] == ' ')
            map[pos] = '.';
    }

    bool eatenCheck(Map& map) {
        if (map[pos] != '.')
            return true;
        return false;
    }

    int getPos() { return pos; }
};

typedef std::list<Food> FoodList;
#endif
