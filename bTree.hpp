#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cstring>
#include <string>
#include <cstdio>
using namespace std;

#define DATA_FILE ceps

#define MAXIMO_CHAVES 4 /* Ordem 5 */
/* Pequena macro para eliminar quebra de linha de WindowsÆ */
#define LIMPAR_QUEBRA(x) if(x[strlen(x)-2] == 0x0D) x[strlen(x)-2] = '\0' //se EndOfLine (ajeitar)

/* ****************** DECLARA«ES DE TIPOS ****************** */

/* Registro de elemento da arvore */
typedef struct entrada{
    int id;
    int byte_offset;
} entrada;

typedef struct tRegistro {
    int id;
    string titulo;
    string genero;
} tRegistro;

typedef struct Pagina { 
    unsigned short num_chaves;
    
    entrada entradas[MAXIMO_CHAVES];

    //int id[MAXIMO_CHAVES];
    //int byte_offset[MAXIMO_CHAVES]; //byte offset do id correspondente
    
    int ponteiros[MAXIMO_CHAVES+1]; //RRN da pagina apontada
} Pagina;


typedef struct Arvore {
    int paginas;
    int raiz;
    int ponteiro;
    FILE *fp;
} Arvore;

#define TAMANHO_PAGINA sizeof(Pagina) //1024

#ifdef MEMORIA
/* Buffer de memória */
char buffer[2000000] = {};
#endif

int pagina_escrever(Arvore *arv, Pagina *pag, int pagina); //ok

int pagina_ler(Arvore *arv, Pagina *pag, int pagina); //ok

int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai); 

int pagina_inserir(Arvore *arv, Pagina *pag, entrada elem); // ok

int arvore_inserir(Arvore *arv, entrada elem); //ok

//Faz a busca do id na árvore, já printando uma struct com as informações
void busca(Arvore *arv, int idBusca); 

int arvore_busca(Arvore *arv, int idBusca); //ok

void arvore_iniciar(Arvore *arv, bool build); //ok

//leitura do arquivo de dados
char *parser(char *buffer, int *pos);
void arquivo_ler(Arvore *arv, int offset);

//escrita no arquivo de dados
int getStrSize(tRegistro *reg, char *buffer);
void arquivo_escrever(tRegistro *reg);

void arvore_build(Arvore *arv);

void arvore_imprimir(Arvore *arv);

int rrnToOffset(int rrn);