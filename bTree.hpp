#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <cstdio> //file functions
#include <iostream> //cin, cout, cerr, clog
#include <sstream> //stringstream
using namespace std;

#define DATA_FILE "dados.dat" //nome arquivo de dados
#define INDEX_FILE "indices.dat" //nome arquivo de indices
#define LOG_FILE "log_EArantes.txt"

#define MAXIMO_CHAVES 4 /* Ordem 5 */
#define TAMANHO_PAGINA (4*(3*(MAXIMO_CHAVES+1) - 1)) //56 pra ordem 5, 4 é sizeof(int)

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
    int num_chaves;
    entrada entradas[MAXIMO_CHAVES];
    int ponteiros[MAXIMO_CHAVES+1]; //RRN da pagina apontada
} Pagina;

typedef struct Arvore {
    int paginas;
    int raiz;
    int ponteiro;
    FILE *fp;
} Arvore;

typedef struct Flag {
    int flagOk;
    int raiz;
} Flag;

#ifdef DEBUG
    #define debug "Chegou aqui\n"
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
    Checa se a ordenacao da pagina esta correta
    Nao muda-se os ponteiros de lugar pois só ocorre em pag nova
    Onde os ponteiros serao -1
    */
void pagina_check(Pagina *pag);


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
void arvore_iniciar(Arvore *arv, bool build, FILE *fp, Flag *flagTmp); //ok


    /*
    Constrói a árvore à partir do arquivo de dados
    */
void arvore_build(Arvore *arv, FILE *fp);




    // Função para o cálculo do Byte_Offset à partir do rrn dado.
int rrnToOffset(int rrn);

    //escrita no arquivo de dados
int getStrSize(tRegistro *reg, char *buffer);
int arquivo_escrever(tRegistro *reg);

/* Funções de manipulação do arquivo de dados */
    //leitura do arquivo de dados
char *parser(char *buffer, int *pos);
tRegistro arquivo_ler(Arvore *arv, FILE*fp, int *offset);

/* Funções chamadas pelo menu*/
    /*
    Insere as determinadas informações no arquivo de dados e na árvore
    tmpId é o id inserido
    title é o título inserido
    gender é o genero inserido
    */
void insercao(Arvore *arv, int tmpId, string title, string gender);

//Faz a busca do id na árvore, já printando uma struct com as informações
void busca(Arvore *arv, int idBusca, FILE *fp); 


    /*
    Imprime a árvore de modo iterativo
    */
void arvore_imprimir(Arvore *arv);

//Printa o menu
void printMenu();

void arvore_debug(Arvore *arv);

void pagina_imprimir(Arvore *arv, int idPag);