#include "bTree.hpp"

int main(void) { 
    long offset = 0;
    char buff[1000] = {};
    FILE *ceps = NULL;
    FILE *consultas = NULL;
    tRegistro temp;
    Arvore arvore;

#ifdef DEBUG
    Pagina pag;
    int j;
    int i;
#endif

    arvore_iniciar(&arvore);
    
    arvore.fp = fopen("indices.dat", "rb+");
    if(arvore.fp == NULL)
        arvore.fp = fopen("indices.dat", "wb+"); //ajeitar nome arquivo (talvez rb+)

    ceps = fopen("dados.dat", "rb"); //ajeitar nome arquivo

    //MONTANDO ARVORE
    while(!feof(ceps)) { // 0 se final do arquivo
        offset = ftell(ceps);
        int ids;
        string title;
        string gen;

        fgets(buff, 1000, ceps);
        
        memcpy(temp.CEP, buff, 8);
        
        //temp.CEP[8] = '\0';
        //AJEITAR

        temp.id = ids;
        temp.titulo = title;
        temp.genero = gen;
        temp.byte_offset = offset; /* Pega o byte offset, adquirido anteriormente. */
        arvore_inserir(&arvore, temp);
    }

#ifdef DEBUG
    printf("Raiz: %d\n", arvore.raiz);
    printf("Imprimindo %d paginas\n", arvore.paginas);
    for(i = 0; i < arvore.paginas; i++) {
        printf("Imprimindo p·gina %d\n", i);
        pagina_ler(&arvore, &pag, i);
        for(j = 0; j < pag.num_chaves; j++) {
            printf("ponteiro[%d]: = %d\nelemento[%d] = %s\n", j, pag.ponteiros[j], j, pag.entradas[j].CEP);
        }
        printf("ponteiro[%d]: = %d\n", j, pag.ponteiros[j]);
        printf("--------------------\n");
    }
#endif

    consultas = fopen("consultas.txt", "r");
    while(!feof(consultas)) {
    	fgets(buff, 1000, consultas);
        LIMPAR_QUEBRA(buff);
 
		/*
        offset = arvore_busca(&arvore, buff);
        */ 
        // AJEITAR AQUI
        
        if(offset < 0) {
            printf("%s NAO ENCONTRADO\n", buff);
        }
        else {
            fseek(ceps, offset, SEEK_SET);
            fgets(buff, 1000, ceps);
            LIMPAR_QUEBRA(buff);
            printf("%s\n", buff);
        }
    }
    fclose(arvore.fp);
    fclose(consultas);
    fclose(ceps);
    return 0;
}
