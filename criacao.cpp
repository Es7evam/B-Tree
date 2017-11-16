#include "criacao.hpp"

void inicializar_BTree(BTree *bt){
	Registro reg = new Registro();
	
	reg.id = 0;
	for(int i=0;i<ORDEM-1;i++){
		reg.chave[i] = 0;
		reg.regs[i] = NULL;
	}
	reg.regs[ORDEM-1] = NULL;
	
	bt->raiz = reg;

	//salvar no arquivo
}

void criar_Indice(){
	FILE *fd, *fp;

	fd = fopen("arvore.idx", 'a+');
		if(fd == NULL)
			return -1;

	fp = fopen("dados.dat", 'r+');
		if(fp == NULL)
			return -1;
	fseek(fd, 0, SEEK_END);
	long size1 = ftell(fd);
	rewind(fd);

	fseek(fp, 0, SEEK_END);
	long size2 = ftell(fp);
	rewind(fp);

	buffer1 = (char*) malloc (sizeof(char)*size1);
	buffer2 = (char*) malloc (sizeof(char)*size2);


	fclose(fd);
	fclose(fp);

}
