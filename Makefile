all:
	g++	bTree.cpp main.cpp -o main -lm --std=c++0x

oldC:
	g++ bTree.cpp main.cpp -o main -lm
debug:
	g++ bTree.cpp main.cpp -o main -DDEBUG -lm

run:
	./main
