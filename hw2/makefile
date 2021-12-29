default: interface.o main.o commands.o
	g++ interface.o main.o commands.o -o default

test: interface.o tests.o
	g++ interface.o tests.o -o test

interface.o: interface.cpp
	g++ -c interface.cpp

main.o: main.cpp
	g++ -c main.cpp

commands.o: commands.cpp
	g++ -c commands.cpp

tests.o: tests.cpp
	g++ -c tests.cpp