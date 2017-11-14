#include "bibliotecas.hpp"
#define ORDEM 5

struct BTree{
	int ordem;
	Registro *raiz;
};

struct Registro {
	int id;
	char titulo[30];
	char genero[20];
};

void inserir();
void busca();
void split();
void concatenar();
void redistribuir();