#include "bibliotecas.hpp"
#define ORDEM 5

struct Arvore{
	short raiz;
	short paginas;
	FILE *fp;
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