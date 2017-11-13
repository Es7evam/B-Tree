#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int id;
	char titulo[30];
	char genero[20];
}Registro;

void checar_vazia();
void criar_arvore();
void inserir();
void busca();
void split();
void concatenar();
void redistribuir();