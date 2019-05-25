CXXFLAGS = -g -I../ncurses-5.9/include -I/usr/include/python3.6m -Wall -DNO_CURSES
LDFLAGS = -L/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu
all: start

start: basic.o Map.o PolicyMaker.o Game.o main.o
	$(CXX) $(CXXFLAGS) $^ -o $@  $(LDFLAGS) -lcurses -lpython3.6m
basic.o: basic.cpp
Map.o: Map.cpp
PolicyMaker.o: PolicyMaker.cpp
Game.o: Game.cpp PolicyMaker.cpp AgentsMgr.h ./agents/b06901145.h ./agents/pyb06901145.h
main.o: main.cpp

clean:
	rm basic.o Map.o PolicyMaker.o Game.o main.o

