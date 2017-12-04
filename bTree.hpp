#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <cstdio> //file functions
#include <iostream> //cin, cout, cerr, clog
#include <sstream> //stringstream
using namespace std;

#define DATA_FILE "dados.dat" //nome arquivo de dados
#define INDEX_FILE "indices.idx" //nome arquivo de indices
#define LOG_FILE "log_EArantes.txt"

#define MAXIMO_CHAVES 4 /* Ordem 5 */
#define TAMANHO_PAGINA (4*(3*(MAXIMO_CHAVES+1) - 1)) //56 pra ordem 5, 4 eh sizeof(int)

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
    int paginas;
    int ponteiro;
} Flag;

#ifdef DEBUG
    #define debug "Chegou aqui\n"
#endif

/* Funcoes de pagina */
    /*
    Escreve a informacao de pag na pagina de determinado numero
    Retorna o 0 se falhar, caso contrario retorna o novo numero da pagina
    */
int pagina_escrever(Arvore *arv, Pagina *pag, int pagina); //ok

    /*
    Lê a pagina com numeracao determinada e guarda a informacao em pag
    Retorna o numero da pagina caso for sucesso.
    */
int pagina_ler(Arvore *arv, Pagina *pag, int pagina); //ok

    /*
    Checa se a ordenacao da pagina esta correta
    Nao muda-se os ponteiros de lugar pois so ocorre em pag nova
    Onde os ponteiros serao -1
    */
void pagina_check(Pagina *pag);


    /*
    Faz o split da pagina, ja redistribuindo as chaves e offsets
    Retorna o pai das paginas splitadas (pode ser uma das duas).      
    */
int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai);

    /*
    Insere um determinado elemento (elem, conjunto de id e offset)
    A insercao eh feita na pagina pag.
    Retorna a posicao onde sera feita a insercao do elemento
    */ 
int pagina_inserir(Arvore *arv, Pagina *pag, entrada elem); // ok



/* Funcoes da arvore */

    /*
    Insere elem (par de id e offset) na determinada arvore
    Todo o tratamnto eh feito dentro dessa funcao.
    eh feita a leitura, split caso necessario, leitura e escrita.
    Tais acoes sao realizadas atravehs da chamada das funcoes de paginas.
    Retorna a pagina caso ocorra bem, caso contrario retorna 0.
    */
int arvore_inserir(Arvore *arv, entrada elem); //ok


    /*
    Busca o idBusca dentro da arvore.
    Usa chamadas das funcoes de paginas para isso, de modo iterativo 
    Retorna o offset do id respectivo, caso nao encontre retorna -1.
    */
int arvore_busca(Arvore *arv, int idBusca); //ok


    /*
    Inicializa a arvore.
    Se build for true, ela construira a arvore a partir do arquivo de dados
    Se build for false, apenas inicializa a arvore vazia.
    */
void arvore_iniciar(Arvore *arv, bool build, FILE *fp, Flag *flagTmp); //ok


    /*
    Constroi a arvore a partir do arquivo de dados
    */
void arvore_build(Arvore *arv, FILE *fp, Flag *fflag);



    /* 
    Funcao para o calculo do Byte_Offset a partir do rrn dado.
    */
int rrnToOffset(int rrn);

/* Funcoes de manipulacao do arquivo de dados */

    /*
    Monta um buffer com o conteudo do registrador
    Retorna o tamanho do tal buffer para ser escrito
    */
int getStrSize(tRegistro *reg, char *buffer);


    /*
    Escreve o determinado registro no arquivo
    Usa a funcao acima para gerar o buffer para a escrita
    */
int arquivo_escrever(tRegistro *reg);


    /*
    Faz o parsing do buffer e muda a posicao de acordo
    */
char *parser(char *buffer, int *pos);


    /*
    Lê no arquivo fp e retorna o registro
    Usa a funcao de parsing para obter o registro
    */
tRegistro arquivo_ler(Arvore *arv, FILE*fp, int *offset);


/* Funcoes chamadas pelo menu*/

    /*
    Insere as determinadas informacoes no arquivo de dados e na arvore
    tmpId eh o id inserido
    title eh o titulo inserido
    gender eh o genero inserido
    */
void insercao(Arvore *arv, int tmpId, string title, string gender, Flag *fflag);


    /*
    Faz a busca do id na arvore
    Printa uma struct com as informacoes buscadas no log
    */
void busca(Arvore *arv, int idBusca, FILE *fp, Flag *fflag); 


    /*
    Imprime a arvore usando uma bfs
    Feita por niveis portanto, usando uma fila
    */
void arvore_imprimir(Arvore *arv, Flag *fflag);


    /*
    Imprime o menu para a interacao com o usuario
    */
void printMenu();


    /*
    Funcao para debug
    Imprime algumas informacoes sobre a arvore
    */
void arvore_debug(Arvore *arv);


    /*
    Funcao para debug
    Imprime a pagina de RRN passado
    */
void pagina_imprimir(Arvore *arv, int idPag);

/* Funcoes relativas a flag para checar se houve problema no problema */

    /*
    Salva as informacoes da arvore no arquivo
    Coloca a flag como verdadeira
    */
void setFlagFalse(Arvore *arv, Flag *fflag);

    
    /*
    Coloca a flag como true e escreve no arquivo a mudanca
    */
void setFlagTrue(Arvore *arv, Flag *fflag);

    /*
    Checa se houve alguma corrupcao no arquivo
    Retorna 1 se houver
    0 caso contrario
    */
int checkFlag(Arvore *arv, Flag *fflag);