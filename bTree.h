#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXIMO_CHAVES 4 /* Ordem 5 */

/* Pequena macro para eliminar quebra de linha de WindowsÆ */
#define LIMPAR_QUEBRA(x) if(x[strlen(x)-2] == 0x0D) x[strlen(x)-2] = '\0'
/* Macro para preencher com -1 um vetor. Usado para "anular" ponteiros
 * das p·ginas */
#define NULLIFICAR(x) memset(x, -1, (MAXIMO_CHAVES+1)*sizeof(short))

/* ****************** DECLARA«ES DE TIPOS ****************** */

/* Registro de elemento da ·rvore */
typedef struct tRegistro {
    int id;
    string titulo;
    string genero;
    unsigned int byte_offset;
} tRegistro;

typedef struct Pagina { 
    int id[MAXIMO_CHAVES];
    int byte_offset[MAXIMO_CHAVES]; //byte offset do id correspondente
    
    unsigned short num_chaves;
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

int pagina_ler(Arvore *arv, Pagina *pag, int pagina);

int pagina_inserir(Arvore *arv, Pagina *pag, tRegistro elem);

void arvore_iniciar(Arvore *arv);

int pagina_escrever(Arvore *avr, Pagina *pag, int pagina);
