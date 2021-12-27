default: interface.o main.o
	g++ interface.o main.o -o default

test: interface.o tests.o
	g++ interface.o tests.o -o test

interface.o: interface.cpp
	g++ -c interface.cpp

main.o: main.cpp
	g++ -c main.cpp

tests.o: tests.cpp
	g++ -c tests.cpp