import ctypes
import sys
import random

MOVE = {
    'noAct': 0,
    'U_Act': 1,
    'D_Act': 2,
    'L_Act': 3,
    'R_Act': 4,
    'ACC_Act': 5
}

class view:
    def __init__(self,map):
        self.map = map
        self.isFood = self.getFood(3)
        self.isWall = self.getWall(3)
        
    def getFood(self,rad):
        isFood = []
        if rad == 1 :
            for x in {16,17,18,23,24,25,30,31,32} :
                if self.map[x] == ".":
                    isFood = isFood + [1]
                else:
                    isFood = isFood + [0]
        elif rad == 2 :
            for x in {8,9,10,11,12,15,16,17,18,19,22,23,24,25,26,29,30,31,32,33,36,37,38,39,40} :
                if self.map[x] == ".":
                    isFood = isFood + [1]
                else:
                    isFood = isFood + [0]
        else:
            for x in range(49):
                if self.map[x] == ".":
                    isFood = isFood + [1]
                else:
                    isFood = isFood + [0]
        self.isFood = isFood
        return isFood


    def getWall(self,rad):
        isWall = []
        if rad == 1 :
            for x in {16,17,18,23,24,25,30,31,32} :
                if self.map[x] == "#":
                    isWall = isWall + [1]
                else:
                    isWall = isWall + [0]
        elif rad == 2 :
            for x in {8,9,10,11,12,15,16,17,18,19,22,23,24,25,26,29,30,31,32,33,36,37,38,39,40} :
                if self.map[x] == "#":
                    isWall = isWall + [1]
                else:
                    isWall = isWall + [0]
        else:
            for x in range(49):
                if self.map[x] == "#":
                    isWall = isWall + [1]
                else:
                    isWall = isWall + [0]

        self.isWall = isWall
        return isWall



class pyAgent:
    def __init__(self):
        view = []
    # void getView(int rad);
    # void getFoodInView(int rad);
    # void getSnakeInView(int rad);
    
    def isOppDir(self, a1, a2):
        if (a1 == U_Act and a2 == D_Act) or (a1 == D_Act and a2 == U_Act) or (a1 == R_Act and a2 == L_Act) or (a1 == R_Act and a2 == R_Act) :
            return true
        return false
    
    
def randMove():
    r = random.randint(0,6)
    # 2/6%: just go ahead
    # 4/6%: 1/6% for each dir
    
    if( r == 0 ):
        return MOVE['noAct']
    elif( r == 1 ):
        return MOVE['U_Act']
    elif( r == 2 ):
        return MOVE['D_Act']
    elif( r == 3 ):
        return MOVE['L_Act']
    elif( r == 4 ):
        return MOVE['R_Act']
    elif( r == 5 ):
        return MOVE['ACC_Act']
    else:
        return MOVE['noAct']

# !! TODO 5: implement your own actionToDo function here
def actionToDo(arg):
    # !! Here are some example for python to get view
    # map = f.readline()
    # print("map",map)
    
    # View = view(map)
    # View.getFood(2)
    # View.getWall(2)
    
    # view (radius = 1):
    # 00 01 02
    # 03 me 05
    # 06 07 08
        
    # view (radius = 2):
    # 00 01 02 03 04
    # 05 06 07 08 09
    # 10 11 me 13 14
    # 15 16 17 18 19
    # 20 21 22 23 24
    # Max radius is 3
    
    # view (radius = 3):
    # 00 01 02 03 04 05 06
    # 07 08 09 10 11 12 13
    # 14 15 16 17 18 19 20
    # 21 22 23 me 25 26 27
    # 28 29 30 31 32 33 34
    # 35 36 37 38 39 40 41
    # 42 43 44 45 46 47 48
    
    return randMove()

f = open(sys.argv[1],'r')

print(actionToDo(1))

