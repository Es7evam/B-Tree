#include "bibliotecas.hpp" 
#define ORDEM 5

struct BTree{
	int ordem;
	Registro *raiz;
};

struct pair{
	int key;
	int offset;
};
	
struct Registro {
	int id;
	pair chave[ORDEM-1];
	Registro* regs[ORDEM];
};

void inserir();
void busca();
void split();
void concatenar();
void redistribuir();

char msg1 = "Execucao da CRIACAO do arquivo de indice arvore.idx com base no arquivo de dados.dat."
char msg2a = "Execucao de operacao de INSERCAO de <ID>, <titulo>, <genero>.";
char msg2b = "Divisao de no - pagina X.";
char msg2c = "Chave <ID> promovida.";
char msg2d = "Chave <ID> inserida com sucesso.";
char msg2e = "Chave <ID> duplicada.";
char msg3a = "Execucao de operacao de PESQUISA de <ID>.";
char msg3b = "Chave <ID> encontrada, offset <Y>, Titulo: <titulo>, Genero: <genero>.";
char msg3c = "Chave <ID> nao encontrada.";
char msg4a = "Execucao de operaca de REMOCAO de <ID>.";
char msg4b = "Redistribuicao de chaves - entre as paginas irmas X e Y.";
char msg4c = "Concatenacao - entre as paginas irmas X e Y.";
char msg4d = "Chave <ID> promovida.";
char msg4e = "Chave <ID> rebaixada.";
char msg4f = "Chave <ID> removida com sucesso.";
char msg4g = "Chave <ID> nao cadastrada.";
char msg5 = "Execucao de operacao para MOSTRAR a arvore-B gerada:";