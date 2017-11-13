#include "bibliotecas.hpp"
#define ORDEM 5

typedef struct Arvore{
	short raiz;
	short paginas;
	FILE *fp;
}

typedef struct Registro {
	int id;
	char titulo[30];
	char genero[20];
}Registro;

void inserir();
void busca();
void split();
void concatenar();
void redistribuir();