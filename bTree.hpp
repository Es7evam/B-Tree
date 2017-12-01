#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cstring>
#include <string>
#include <cstdio>
using namespace std;

#define MAXIMO_CHAVES 4 /* Ordem 5 */

/* Pequena macro para eliminar quebra de linha de WindowsÆ */
#define LIMPAR_QUEBRA(x) if(x[strlen(x)-2] == 0x0D) x[strlen(x)-2] = '\0' //se EndOfLine (ajeitar)

/* ****************** DECLARA«ES DE TIPOS ****************** */

/* Registro de elemento da ·rvore */
typedef struct tRegistro {
    int id;
    string titulo;
    string genero;
    unsigned int byte_offset; // tamanho
} tRegistro;

typedef struct Pagina { 
    unsigned short num_chaves;
    
    int id[MAXIMO_CHAVES];
    int byte_offset[MAXIMO_CHAVES]; //byte offset do id correspondente
    
    int ponteiros[MAXIMO_CHAVES+1]; //RRN da pagina apontada
} Pagina;


typedef struct Arvore {
    int paginas;
    int raiz;
    int ponteiro;
    FILE *fp;
} Arvore;

#define TAMANHO_PAGINA sizeof(Arvore) //1024

#ifdef MEMORIA
/* Buffer de memÛria */
char buffer[2000000] = {};
#endif

int pagina_escrever(Arvore *arv, Pagina *pag, int pagina); //ok

int pagina_ler(Arvore *arv, Pagina *pag, int pagina); //ok

int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai); 

int pagina_inserir(Arvore *arv, Pagina *pag, tRegistro elem); // ok

int arvore_inserir(Arvore *arv, tRegistro elem); //ok caso split MAXIMO

int arvore_busca(Arvore *arv, int idBusca); //ok

void arvore_iniciar(Arvore *arv); //ok

void arvore_imprimir(Arvore *arv);