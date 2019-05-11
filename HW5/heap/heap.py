HEAPSIZE = 500000
class Node:
    def __init__(self):
        self.key = 0
        self.element = []

class Heap:
    def __init__(self):
        self.size=0
        self.ary=[ Node() for _ in range(HEAPSIZE)]

    def printArray(self):
        print("(Index, Key, Element)")
        for i in range(self.size):
            print("({},{},{})".format(str(i),str((self.ary)[i].key),str((self.ary)[i].element)))

    def isempty(self):
        return ((self.size)==0)

    def printByPopping(self):
        while not (self.isempty()):
            print(self.pop(),end='')
        print("")

    def getTopKey(self):
        return (self.ary)[0].key

    def swap(self,i,j):
        (self.ary)[i], (self.ary)[j] = (self.ary)[j], (self.ary)[i] 
    def push(self,key,element):
	    pass
        #todo
    def pop(self):
        pass
        #todo
