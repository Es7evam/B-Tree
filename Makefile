all:
	g++	bTree.cpp main.cpp -o main -lm

debug:
	g++ bTree.cpp main.cpp -o main -DDEBUG -lm

run:
	./main