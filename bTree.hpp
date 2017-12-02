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

/* Funções de página */
    /*
    Escreve a informação de pag na página de determinado número
    Retorna o 0 se falhar, caso contrário retorna o novo número da página
    */
int pagina_escrever(Arvore *arv, Pagina *pag, int pagina); //ok

    /*
    Lê a página com numeração determinada e guarda a informação em pag
    Retorna o número da página caso for sucesso.
    */
int pagina_ler(Arvore *arv, Pagina *pag, int pagina); //ok

    /*
    Faz o split da página, já redistribuindo as chaves e offsets
    Retorna o pai das páginas splitadas (pode ser uma das duas).      
    */
int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai);

    /*
    Insere um determinado elemento (elem, conjunto de id e offset)
    A inserção é feita na página pag.
    Retorna a posição onde será feita a inserção do elemento
    */ 
int pagina_inserir(Arvore *arv, Pagina *pag, entrada elem); // ok



/* Funções da árvore */

    /*
    Insere elem (par de id e offset) na determinada árvore
    Todo o tratamnto é feito dentro dessa função.
    É feita a leitura, split caso necessário, leitura e escrita.
    Tais ações são realizadas através da chamada das funções de paginas.
    Retorna a pagina caso ocorra bem, caso contrário retorna 0.
    */
int arvore_inserir(Arvore *arv, entrada elem); //ok

    /*
    Busca o idBusca dentro da árvore.
    Usa chamadas das funções de páginas para isso, de modo iterativo 
    Retorna o offset do id respectivo, caso não encontre retorna -1.
    */
int arvore_busca(Arvore *arv, int idBusca); //ok

    /*
    Inicializa a árvore.
    Se build for true, ela construirá a árvore à partir do arquivo de dados
    Se build for false, apenas inicializa a árvore vazia.
    */
void arvore_iniciar(Arvore *arv, bool build); //ok

    /*
    Constrói a árvore à partir do arquivo de dados
    */
void arvore_build(Arvore *arv);

    /*
    Imprime a árvore de modo iterativo
    */
void arvore_imprimir(Arvore *arv);


// Função para o cálculo do Byte_Offset à partir do rrn dado.
int rrnToOffset(int rrn);

/* Funções de manipulação do arquivo de dados */
//leitura do arquivo de dados
char *parser(char *buffer, int *pos);
void arquivo_ler(Arvore *arv, FILE*fp, int *offset);

//escrita no arquivo de dados
int getStrSize(tRegistro *reg, char *buffer);
void arquivo_escrever(tRegistro *reg);


/* Funções chamadas pelo menu*/
void insercao(Arvore *arv, int tmpId, string title, string gender);

//Faz a busca do id na árvore, já printando uma struct com as informações
void busca(Arvore *arv, int idBusca); 
