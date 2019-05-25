CXXFLAGS = -g -I../ncurses-5.9/include -Wall -DNO_CURSES
all: start

start: basic.o Map.o PolicyMaker.o Game.o main.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -lcurses
basic.o: basic.cpp
Map.o: Map.cpp
PolicyMaker.o: PolicyMaker.cpp
Game.o: Game.cpp PolicyMaker.cpp AgentsMgr.h ./agents/b06901145.h ./agents/pyb06901145.h
main.o: main.cpp

clean:
	rm basic.o Map.o PolicyMaker.o Game.o main.o

