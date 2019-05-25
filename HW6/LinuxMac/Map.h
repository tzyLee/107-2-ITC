#ifndef __Map_h__
#define __Map_h__

#include "basic.h"
#include <string>
#include <iostream>
#include<fstream>

using namespace std;

class Map {
   private:
    int width;
    int height;
    chtype* data;  // curses char type (unsigned long)

    void copy(const Map& orig);

   public:
    void newMap(int w = 120, int h = 40);
    void copy_local(const Map& orig, int centerPos, int rad);
    Map(int w = 120, int h = 40) { newMap(w, h); }
    Map(const Map& orig) { copy(orig); }
    Map(const Map& orig, int centerPos, int rad) {
        copy_local(orig, centerPos, rad);
    }
    Map& operator=(const Map& orig) {
        clear();
        copy(orig);
        return *this;
    }
    void clear() { delete[] data; }
    ~Map() { clear(); }

    chtype& operator[](int pos) { return data[pos]; }
    void print() const;
    bool isEdge(const int pos) const;
    bool isFood(const int pos) const;
    bool isWall(const int pos) const;
    int XY2Pos(const int x, const int y) const { return y * width + x; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    
     string getMapStr(){
	int size = width*height;
        string str="";
	for(int i=0;i<size;i++) {
        
                  str+=data[i];
        }
        return str;
		
     };
    
    
};

#endif  // __Map_h__
