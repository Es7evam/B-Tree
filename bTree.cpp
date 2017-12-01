#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bTree.hpp"

int pagina_escrever(Arvore *arv, Pagina *pag, int pagina) {
#ifdef MEMORIA
    if(pagina < 0) {
        arv->ponteiro += TAMANHO_PAGINA;
        memcpy(&buffer[arv->ponteiro], pag, TAMANHO_PAGINA);
        return arv->ponteiro / TAMANHO_PAGINA;
    }
    else {
        memcpy(&buffer[pagina*TAMANHO_PAGINA], pag, TAMANHO_PAGINA);
        return pagina;
    }
#else
    if(pagina < 0) {
        arv->ponteiro += TAMANHO_PAGINA;
        fseek(arv->fp, 0, SEEK_END);
        fwrite(pag, TAMANHO_PAGINA, 1, arv->fp);
        return arv->ponteiro / TAMANHO_PAGINA;
    }
    else {
        fseek(arv->fp, pagina*TAMANHO_PAGINA, SEEK_SET);
        fwrite(pag, TAMANHO_PAGINA, 1, arv->fp);
        return pagina;
    }
#endif
    return 0;
}

int pagina_ler(Arvore *arv, Pagina *pag, int pagina) {
#ifdef MEMORIA
    memcpy(pag, &buffer[pagina*TAMANHO_PAGINA], TAMANHO_PAGINA);
    return pagina;
#else
    fseek(arv->fp, pagina*TAMANHO_PAGINA, SEEK_SET);
    fread(pag, TAMANHO_PAGINA, 1, arv->fp);
    return pagina;
#endif
}

int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai) {
    Pagina pag_nova;
    Pagina pag_pai;
    int local = 0; /* local onde ser· inserido o elemento promovido */
    int nova = 0; /* nro nova página */
    int medio = 0; /* posiçao mediana */
    int i = 0;

    /* Iniciar as paginas novas
     * Se o pai ja existir, lê, caso contrário cria*/
    if(pai >= 0) {
        pagina_ler(arv, &pag_pai, pai);
    }
    else {
        pag_pai.num_chaves = 0;
        memset(pag_pai.id, 0, MAXIMO_CHAVES*sizeof(int));
    	memset(pag_pai.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));
    }

    /* Cria a nova pagina */
    pag_nova.num_chaves = 0;
    memset(pag_nova.id, 0, MAXIMO_CHAVES*sizeof(int));
    memset(pag_nova.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));    
    
    //minimo de elementos
    medio = (MAXIMO_CHAVES-1)/2;

    /* Insere os elementos da pagina a sofrer 'splitting' na pagina
     * nova e remove os mesmos da pagina original. */
    for(i = medio+1; i < MAXIMO_CHAVES; i++) {
        /* i-(medio+1) faz com que o laÁo comece em 0 para
         * a pagina nova, que vai receber os novos elementos. */
        pag_nova.entradas[i-(medio+1)] = pag->entradas[i];
        pag->entradas[i].CEP[0] = '\0'; /* Apaga o elemento, 
                                           anulando a string. */

        /* Move agora os ponteiros. */
        pag_nova.ponteiros[i-medio] = pag->ponteiros[i+1];
        pag->ponteiros[i+1] = -1;
    }
    pag_nova.num_chaves = medio;
    pag->num_chaves = medio;

    /* Move agora o ponteiro final de pag para nova */
    pag_nova.ponteiros[0] = pag->ponteiros[medio+1];
    pag->ponteiros[medio+1] = -1;

    nova = pagina_escrever(arv, &pag_nova, -1);

    /* InÌcio do "promoting". */
    /* Agora insere o elemento mediano na pagina pai. */
    local = pagina_inserir(arv, &pag_pai, pag->entradas[medio]);
    pag->entradas[medio].CEP[0] = '\0';

    /* Coloca os ponteiros no pai. */
    pag_pai.ponteiros[local] = pagina;
    pag_pai.ponteiros[local+1] = nova;
    
    /* Atualiza a ·rvore */
    if(pai == -1) 
    	arv->paginas += 2; 
    else 
    	arv->paginas++;

    pai = pagina_escrever(arv, &pag_pai, pai);
    pagina_escrever(arv, pag, pagina);

#ifdef DEBUG
    printf("Split: %d para %d e %d\n", pagina, nova, pai);
#endif

    if(pagina == arv->raiz) {
        arv->raiz = pai;
#ifdef DEBUG
        printf("Atualizando raiz: %d\n", arv->raiz);
#endif
    }

    return pai;
}

int pagina_inserir(Arvore *arv, Pagina *pag, tRegistro elem) {
    int finalizado = 0;
    int posicao_ideal = -1;

    pag->num_chaves++;
    int i;
    /* Primeiramente, procurar por um lugar vazio no vetor. */
    for(i=0; i < pag->num_chaves && !finalizado; i++ ) {
    	if(id[i] - elem.id > 0){ 
        //if(strcmp(pag->entradas[i].CEP, elem.CEP) > 0)  {
            posicao_ideal = i;
            finalizado = 1;
        }
    }
    /* Se no encontrou posiÁo ideal e h· mais de uma chave
     * no vetor, ento a posiÁo ideal È no final do vetor. */
    if(posicao_ideal < 0) {
        posicao_ideal = pag->num_chaves-1;
    }

    i = posicao_ideal;

    /* Procura um espaÁo vazio. */
    while(pag->id[i] != 0) 
    	i++;

    while(posicao_ideal != i) {
        pag->ponteiros[i+1] = pag->ponteiros[i]; /* move o ponteiro adiantado */
        pag->ponteiros[i] = -1;

        pag->entradas[i] = pag->entradas[i-1];
        i--;
    }

    /* Move os ponteiros e finalmente aloca o elemento em sua posiÁo. */
    pag->ponteiros[posicao_ideal+1] = pag->ponteiros[posicao_ideal];
    pag->ponteiros[posicao_ideal] = -1;
    pag->entradas[posicao_ideal] = elem;

    return posicao_ideal;
}

int arvore_inserir(Arvore *arv, tRegistro elem) {
    int pagina_pai = -1;
    int pagina_atual;
    int fim_pagina = 0;
    Pagina pag;

    pagina_ler(arv, &pag, arv->raiz);
    pagina_atual = arv->raiz;

    if(pagina_atual == arv->raiz && pag.num_chaves == MAXIMO_CHAVES) {
    	//fazendo split da raiz se necessario
        pagina_atual = pagina_split(arv, &pag, pagina_atual, -1);
        pagina_ler(arv, &pag, arv->raiz);
        pagina_pai = -1;
    }

    while(true) {
        for(int i = 0; i <= pag.num_chaves && !fim_pagina; i++) {
            /* Checa-se se È o ?ltimo ponteiro da pagina ou se È um local adequado
             * a se buscar um ponteiro por pagina */
            //if(i == pag.num_chaves || strcmp(pag.entradas[i].CEP, elem.CEP) > 0) {
        	if(i == pag.num_chaves || pag.id[i] - elem.id > 0){
                if(pag.ponteiros[i] != -1) {
                    pagina_pai = pagina_atual;
                    pagina_atual = pag.ponteiros[i];
                    pagina_ler(arv, &pag, pagina_atual);

                    if(pag.num_chaves == MAXIMO_CHAVES) {
                        pagina_atual = pagina_split(arv, &pag, pagina_atual, pagina_pai);
                        pagina_ler(arv, &pag, pagina_atual);
                        pagina_pai = -1;
                    }
                    fim_pagina = 1;
                }
                else {
                    /* Inserir na pagina, caso a mesma no esteja vazia. */
#ifdef DEBUG
                    printf("Inserindo %s na pagina %d\n", elem.CEP, pagina_atual);
#endif
                    pagina_inserir(arv, &pag, elem);
                    pagina_escrever(arv, &pag, pagina_atual);
                    return pagina_atual; /* Ponto de saÌda da funÁo, apÛs a inserÁo. */
                }
            }
        }
        fim_pagina = 0;
    }
    return 0;
}

int arvore_busca(Arvore *arv, int idBusca) { //ok
    Pagina pag;
    int i = 0;
    int finalizado_arvore = 0;
    int finalizado_pagina = 0;

    pagina_ler(arv, &pag, arv->raiz);
    while(!finalizado_arvore) {
        /* Itera entre os ponteiros da ·rvore. */
        for(i = 0; i < pag.num_chaves && !finalizado_pagina; i++) {
        	//se o buscado é menor que o atual entra
            if(pag.id[i] - idBusca > 0) {
                if(pag.ponteiros[i] >= 0) {
                    pagina_ler(arv, &pag, pag.ponteiros[i]);
                    finalizado_pagina = 1;
                } else 
                	return -1; /* Se no existe ponteiro, acabou a busca. */
            } else {
                if(pag.id[i] == idBusca) {
                    return pag.byte_offset[i];
                }
            }
        }
        /* Se saiu do laÁo, ou È porque achamos uma pagina e vamos
         * comeÁar a leitura nela (finalizado_pagina = 1) ou
         * È porque no encontramos candidatos para tal. Fazer uma
         * ?ltima tentativa no ponteiro p+1. Se ele no existir,
         * terminamos uma busca que no achou. */
        if(finalizado_pagina) {
            finalizado_pagina = 0;
        }
        else {
            if(pag.ponteiros[i] >= 0) {
                finalizado_pagina = 0;
                pagina_ler(arv, &pag, pag.ponteiros[i]);
            } else 
            	finalizado_arvore = 1; /* Se no encontramos nada... */
        }
    }
    return -1;
}

void arvore_iniciar(Arvore *arv) { //ok
    Pagina pag;

    arv->paginas = 1;
    arv->raiz = 0;
    arv->ponteiro = -TAMANHO_PAGINA; /* Começa com -tampag para facilitar */
    arv->fp = NULL;
    
    pag.num_chaves = 0;
    memset(pag.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));
    memset(pag.entradas, 0, MAXIMO_CHAVES*sizeof(tRegistro));

    pagina_escrever(arv, &pag, -1);
}

void arvore_imprimir(Arvore *arv){
    Pagina pag;
    int i, j;

    printf("Raiz: %d\n", arv->raiz);
    printf("Imprimindo %d paginas\n", arv->paginas);
    for(i = 0; i < arv->paginas; i++) {
        printf("Imprimindo pagina %d\n", i);
        pagina_ler(arv, &pag, i);
        for(j = 0; j < pag.num_chaves; j++) {
            printf("ponteiro[%d]: = %d\nelemento[%d] = %s\n", j, pag.ponteiros[j], j, pag.entradas[j]->CEP);
        }
        printf("ponteiro[%d]: = %d\n", j, pag.ponteiros[j]);
        printf("--------------------\n");
    }
}