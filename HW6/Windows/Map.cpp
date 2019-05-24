#include<cstdlib>
#include "basic.h"
#include "Map.h"

//private
void Map::copy(const Map& orig){
    width = orig.width;
    height = orig.height;
    int size = width*height;
    data = new chtype[size];
    for(int k=0;k<size;++k)
        data[k] = orig.data[k];
}

//public
void Map::newMap(int w,int h){
    width = w; height = h;
    int size = width*height;
    data = new chtype[size];
    //memset (data,' ',size);
    for(int k=0;k<size;++k){
        if(  k%width==0||k%width==(width-1)  )  data[k]='#';
        else if(  k<width||k>=(size-width)  )   data[k]='#';
        else data[k]= ' ';
    }
}

void Map::copy_local(const Map& orig,int centerPos,int rad){
    width = 1+2*rad;
    height = 1+2*rad;
    int size = width*height;
    data = new chtype[size];

    int centerX = centerPos%orig.width;
    int centerY = centerPos/orig.width;
    int mapX,mapY,viewX,viewY,k=0;
    for(viewY=0,mapY=centerY-rad;viewY<height;++viewY,++mapY){
        for(viewX=0,mapX=centerX-rad;viewX<width;++viewX,++mapX){
            if(  mapX<0||mapX>=orig.width  ||  mapY<0||mapY>=orig.height  )
                data[k] = 'x';
            else
                data[k] = orig.data[orig.XY2Pos(mapX,mapY)];
            ++k;
        }
    }

}

void Map::print()const{
    for(int j=0,k=0;j<height;++j){
        for(int i=0;i<width;++i,++k){
            mvaddch(j,i,data[k]);
        }
        #ifndef NO_CURSES
        #else
        printw("\n");
        #endif
    }
    //refresh();
}

//public util
bool Map::isEdge(const int pos)const{
    if(  pos%width==0||pos%width==(width-1)  )  return true;
    else if(  pos<width||pos>=(width*(height-1))  )   return true;
    else return false;
}
bool Map::isFood(const int pos)const{
    char obj = data[pos] & 0xFF;
    if(obj=='.') return true;
    else return false;
}
bool Map::isWall(const int pos)const{
    char obj = data[pos] & 0xFF;
    if(obj=='#') 
        return true;
    else return false;
}


