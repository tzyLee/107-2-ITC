all: basic.o Map.o PolicyMaker.o Game.o main.o
	g++ -o start basic.o Map.o PolicyMaker.o Game.o main.o -lcurses
	
basic.o: basic.cpp
	g++ -g -I../ncurses-5.9/include -c basic.cpp -o basic.o
Map.o: Map.cpp
	g++ -g -I../ncurses-5.9/include -c Map.cpp -o Map.o
PolicyMaker.o: PolicyMaker.cpp
	g++ -g -I../ncurses-5.9/include -c PolicyMaker.cpp -o PolicyMaker.o
Game.o: Game.cpp
	g++ -g -I../ncurses-5.9/include -c Game.cpp -o Game.o
main.o: main.cpp
	g++ -g -I../ncurses-5.9/include -c main.cpp -o main.o

clean:
	rm basic.o Map.o PolicyMaker.o Game.o main.o

