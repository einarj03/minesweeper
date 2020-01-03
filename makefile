minesweeper: main.o minesweeper.o
	g++ -Wall -g main.o minesweeper.o -o minesweeper

main.o: main.cpp minesweeper.h
	g++ -Wall -c main.cpp

minesweeper.o: minesweeper.cpp minesweeper.h
	g++ -Wall -c minesweeper.cpp