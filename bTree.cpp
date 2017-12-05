#include "bTree.hpp"

int pagina_escrever(Arvore *arv, Pagina *pag, int pagina) {
    if(pagina < 0) { //se for a pagina nova
        arv->ponteiro += TAMANHO_PAGINA;
        //ajeitado rrn aqui
        fseek(arv->fp, 0, SEEK_END);
        fwrite(pag, TAMANHO_PAGINA, 1, arv->fp);
        return arv->ponteiro / TAMANHO_PAGINA;
    }
    else {
        //ajeitado rrn aqui
        fseek(arv->fp, rrnToOffset(pagina), SEEK_SET); //busca do inicio do arquivo
        fwrite(pag, TAMANHO_PAGINA, 1, arv->fp);
        return pagina;
    }
    return 0;
}

int pagina_ler(Arvore *arv, Pagina *pag, int pagina) {
    //ajeitado rrn aqui
    fseek(arv->fp, rrnToOffset(pagina), SEEK_SET);
    fread(pag, TAMANHO_PAGINA, 1, arv->fp);
    return pagina;
}


void pagina_check(Pagina *pag){
    entrada aux;
    bool trocado;
    do{ 
        trocado = false;          
        for(int i=0;i<pag->num_chaves-1;i++){
            if(pag->entradas[i].id > pag->entradas[i+1].id){
                trocado = true;   
#ifdef DEBUG
                cout << "Swap entre " << pag->entradas[i].id << " e " << pag->entradas[i+1].id;
#endif
                aux = pag->entradas[i];
                pag->entradas[i] = pag->entradas[i+1];
                pag->entradas[i+1] = aux;
            }
        }
    }   while(trocado);
}

int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai, entrada elem) {
    Pagina pag_nova;
    Pagina pag_pai;
    int local = 0; /* local onde ser· inserido o elemento promovido */
    int nova = 0; /* nro nova pagina */
    int medio = (MAXIMO_CHAVES)/2; /* posiçao mediana */
    int i = 0;

    /* Iniciar as paginas novas
     * Se o pai ja existir, lê, caso contrario cria*/
    if(pai >= 0) {
        pagina_ler(arv, &pag_pai, pai);
    }
    else {
        pag_pai.num_chaves = 0;
        memset(pag_pai.entradas, 0, MAXIMO_CHAVES*sizeof(entrada));
    	memset(pag_pai.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));
    }

    /* Cria a nova pagina */
    pag_nova.num_chaves = 0;
    memset(pag_nova.entradas, 0, MAXIMO_CHAVES*sizeof(entrada));
    memset(pag_nova.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));    

    i=0;
    bool ok = false;
    entrada tmpEntrada[MAXIMO_CHAVES+1];
    int tmpPonteiro[MAXIMO_CHAVES+2], posicao_ideal = -1;
    for(i=0; i < MAXIMO_CHAVES;i++){
        tmpEntrada[i] = pag->entradas[i];
        tmpPonteiro[i] = pag->ponteiros[i];
    } // Copia para o vetor auxiliar
    tmpPonteiro[i] = pag->ponteiros[i]; // pega o ultimo ponteiro
    

    for(i=0; i < MAXIMO_CHAVES+1 && !ok; i++) {
        if(tmpEntrada[i].id - elem.id > 0){ 
            posicao_ideal = i;
            ok = true;
        }
    }
    //Se no encontrou posiçao ideal e ha mais de uma chave
    // no vetor, entao a posiçao ideal eh no final do vetor.
    if(posicao_ideal < 0) {
        posicao_ideal = MAXIMO_CHAVES-1;
    }

    //Espaço vazio
    i = MAXIMO_CHAVES;

    while(posicao_ideal != i) {
        tmpPonteiro[i+1] = tmpPonteiro[i]; // move o ponteiro adiantado 
        tmpPonteiro[i] = -1;

        tmpEntrada[i] = tmpEntrada[i-1];
        i--;
    }

    //Move os ponteiros e finalmente aloca o elemento em sua posiçao. 
    tmpPonteiro[posicao_ideal+1] = tmpPonteiro[posicao_ideal];
    tmpPonteiro[posicao_ideal] = -1;
    tmpEntrada[posicao_ideal] = elem;

    //minimo de elementos
    /* Insere os elementos da pagina a sofrer 'splitting' na pagina
     * nova e remove os mesmos da pagina original. */
    for(i = medio+1; i < MAXIMO_CHAVES+1; i++) {
        /* i-(medio+1) faz com que o laço comece em 0 para
         * a pagina nova, que vai receber os novos elementos. */
        pag_nova.entradas[i-(medio+1)] = tmpEntrada[i];
        tmpEntrada[i].id = 0; // Apaga o elemento       //AJEITAR
         

        /* Move agora os ponteiros. */
        pag_nova.ponteiros[i-medio] = tmpPonteiro[i+1];
        tmpPonteiro[i+1] = -1;
    }

    pag_nova.num_chaves = medio;
    pag->num_chaves = medio;

    /* Move agora o ponteiro final de pag para nova */
    pag_nova.ponteiros[0] = tmpPonteiro[medio+1];
    tmpPonteiro[medio+1] = -1;

    pagina_check(&pag_nova);
    nova = pagina_escrever(arv, &pag_nova, -1);
#ifdef DEBUG
    cout << "Nova: " << nova << endl;
#endif

    /* Inicio do "promoting". */
    /* Agora insere o elemento mediano na pagina pai. */
    clog << "Chave " << tmpEntrada[medio].id << " promovida" << endl;
    local = pagina_inserir(arv, &pag_pai, tmpEntrada[medio]);
    tmpEntrada[medio].id = 0;


    /* Coloca os ponteiros no pai. */
    pag_pai.ponteiros[local] = pagina;
    pag_pai.ponteiros[local+1] = nova;
    
    //Copia de volta a pagina temporaria
    for(i = 0; i < MAXIMO_CHAVES;i++){
        pag->ponteiros[i] = tmpPonteiro[i];
        pag->entradas[i] = tmpEntrada[i];
    }

    /* Atualiza a arvore */
    if(pai == -1) 
    	arv->paginas += 2; 
    else 
    	arv->paginas++;

    pai = pagina_escrever(arv, &pag_pai, pai);
    pagina_escrever(arv, pag, pagina);

    clog << "Divisao de no - pagina " << pagina << endl;
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

int pagina_inserir(Arvore *arv, Pagina *pag, entrada elem) {
    int finalizado = 0;
    int posicao_ideal = -1;

    pag->num_chaves++;
    int i;
    /* Primeiramente, procurar por um lugar vazio no vetor. */
    for(i=0; i < pag->num_chaves && !finalizado; i++) {
    	if(pag->entradas[i].id - elem.id > 0){ 
            posicao_ideal = i;
            finalizado = 1;
        }
    }
    /* Se no encontrou posiçao ideal e ha mais de uma chave
     * no vetor, ento a posiçao ideal eh no final do vetor. */
    if(posicao_ideal < 0) {
        posicao_ideal = pag->num_chaves-1;
    }

    i = posicao_ideal;

    /* Procura um espaço vazio. */
    while(pag->entradas[i].id != 0) 
    	i++;

    while(posicao_ideal != i) {
        pag->ponteiros[i+1] = pag->ponteiros[i]; /* move o ponteiro adiantado */
        pag->ponteiros[i] = -1;

        pag->entradas[i] = pag->entradas[i-1];
        i--;
    }

    /* Move os ponteiros e finalmente aloca o elemento em sua posiçao. */
    pag->ponteiros[posicao_ideal+1] = pag->ponteiros[posicao_ideal];
    pag->ponteiros[posicao_ideal] = -1;
    pag->entradas[posicao_ideal] = elem;

    return posicao_ideal;
}

int arvore_inserir(Arvore *arv, entrada elem) {
    if(arvore_busca(arv, elem.id) != -1){
        return -1;
    }
    int pagina_pai = -1;
    int pagina_atual;
    int fim_pagina = 0;
    Pagina pag;

    pagina_ler(arv, &pag, arv->raiz);
    pagina_atual = arv->raiz;

    bool splitted = false;
    if(pagina_atual == arv->raiz && pag.num_chaves == MAXIMO_CHAVES) {
    	//fazendo split da raiz se necessario
        pagina_atual = pagina_split(arv, &pag, pagina_atual, -1, elem);
        splitted = true;
        pagina_ler(arv, &pag, arv->raiz);
        pagina_pai = -1;
#ifdef DEBUG
        cout << "Entrou aqui, id = " << elem.id << endl;
#endif
    }

    while(true) {
        for(int i = 0; i <= pag.num_chaves && !fim_pagina; i++) {
            /* Checa-se se eh o ultimo ponteiro da pagina ou se eh um local adequado
             * a se buscar um ponteiro por pagina */
            //if(i == pag.num_chaves || strcmp(pag.entradas[i].CEP, elem.CEP) > 0) { ajeitar
        	if(i == pag.num_chaves || pag.entradas[i].id - elem.id > 0){
                if(pag.ponteiros[i] != -1) {
                    pagina_pai = pagina_atual;
                    pagina_atual = pag.ponteiros[i];
                    pagina_ler(arv, &pag, pagina_atual);

                    if(pag.num_chaves == MAXIMO_CHAVES) {
                        pagina_atual = pagina_split(arv, &pag, pagina_atual, pagina_pai, elem);
                        splitted = true;
                        pagina_ler(arv, &pag, pagina_atual);
                        pagina_pai = -1;
                    }
                    fim_pagina = 1;
                }
                else {
                    /* Inserir na pagina, caso ela nao esteja vazia. */
#ifdef DEBUG
#endif
                    clog << "Chave " << elem.id << " inserida com sucesso" << endl;
                    if(!splitted){
#ifdef DEBUG
                        cout << "Inseriu " << elem.id << " na pagina " << pagina_atual << endl;
#endif
                        pagina_inserir(arv, &pag, elem);
                        pagina_escrever(arv, &pag, pagina_atual);
                    }
#ifdef DEBUG
#endif

                    return pagina_atual; /* Ponto de saida da funçao, apos a inserçao. */
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
    bool finalizado_arvore = false;
    bool finalizado_pagina = false;

    pagina_ler(arv, &pag, arv->raiz);
    while(!finalizado_arvore) {
        /* Itera entre os ponteiros da ·rvore. */
        for(i = 0; i < pag.num_chaves && !finalizado_pagina; i++) {
        	//se o buscado eh menor que o atual entra
            if(pag.entradas[i].id - idBusca > 0) {
                if(pag.ponteiros[i] >= 0) {
                    pagina_ler(arv, &pag, pag.ponteiros[i]);
                    finalizado_pagina = 1;
                } else 
                	return -1; /* Se no existe ponteiro, acabou a busca. */
            } else {
                if(pag.entradas[i].id == idBusca) {
                    return pag.entradas[i].byte_offset;
                }
            }
        }
        /* Se saiu do loop, ou eh porque achamos uma pagina e vamos
         * começar a leitura nela (finalizado_pagina = 1) ou
         * eh porque no encontramos candidatos para tal. Fazer uma
         * ultima tentativa no ponteiro p+1. Se ele no existir,
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

void arvore_iniciar(Arvore *arv, bool build, FILE *fp, Flag *flagTmp) { //ok
    arv->raiz = 0;
    arv->fp = NULL;
    flagTmp->flagOk = 1;

    Pagina pag;
    pag.num_chaves = 0;
    memset(pag.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));
    memset(pag.entradas, 0, MAXIMO_CHAVES*sizeof(entrada));

    arv->fp = fopen(INDEX_FILE, "r+b");
    if(arv->fp == NULL){
        cout << "Nao existia arquivo de indices, foi criado" << endl;
        arv->fp = fopen(INDEX_FILE, "w+b"); //ajeitar nome arquivo (talvez rb+)
        fwrite(flagTmp, sizeof(Flag), 1, arv->fp); //escreve as infos no arquivo arvore
        build = true;
    } else{
        if(fgetc(arv->fp) == EOF){
#ifdef DEBUG
            cout << "Arquivo vazio";
#endif
            rewind(arv->fp);
            fwrite(flagTmp, sizeof(Flag), 1, arv->fp); //escreve as infos no arquivo arvore
            fflush(arv->fp);
            pagina_escrever(arv, &pag, -1);
        }else{
            rewind(arv->fp);

            fread(flagTmp, sizeof(Flag), 1, arv->fp); //le infos da arvore
            if(feof(arv->fp)){
                build = true;
#ifdef DEBUG
                cout << "EOF" << endl;
#endif
            }else{
                arv->raiz = flagTmp->raiz;
                arv->paginas = flagTmp->paginas;
                arv->ponteiro = flagTmp->ponteiro;
                pagina_ler(arv, &pag, arv->raiz); //lê a raiz
            }
        }
    }

#ifdef DEBUG
    pagina_imprimir(arv, 0);
#endif

    if(build){
        fclose(arv->fp);
        arv->fp = fopen(INDEX_FILE, "wb+");
        rewind(fp); //rewind por segurança
        rewind(arv->fp);
        fwrite(flagTmp, sizeof(Flag), 1, arv->fp); //escreve as infos no arquivo arvore
        arvore_build(arv, fp, flagTmp);
    } //ajeitar

    setFlagTrue(arv, flagTmp);
}

void arvore_build(Arvore *arv, FILE *fp, Flag *fflag){
    arv->paginas = 1;    
    arv->ponteiro = -TAMANHO_PAGINA; /* Começa com -tampag para facilitar */
    arv->raiz = 0;

    Pagina raiz;
    pagina_ler(arv, &raiz, 0);
    memset(raiz.ponteiros, -1, (MAXIMO_CHAVES+1)*sizeof(int));
    memset(raiz.entradas, 0, MAXIMO_CHAVES*sizeof(entrada));
    pagina_escrever(arv, &raiz, -1);

    setFlagFalse(arv, fflag);
    int offset;
    //lembrar flag aqui ajeitar
    tRegistro tmp;
    entrada entradaTmp;


    bool go = true;
    if(fgetc(fp) == EOF){
        go = false; //arquivo vazio
    }
    rewind(fp);
    while(!feof(fp) && go){
        //lendo no arquivo
        tmp = arquivo_ler(arv, fp, &offset);
        if(offset == -1){
            break;
        }

        //coloca na arvore
        entradaTmp.id = tmp.id;
        entradaTmp.byte_offset = offset; // Pega o byte offset, adquirido anteriormente.

#ifdef DEBUG
        cout << "Build: " << entradaTmp.id << ": " << entradaTmp.byte_offset << endl;
#endif
        arvore_inserir(arv, entradaTmp);
    }
    setFlagTrue(arv, fflag);
}

int rrnToOffset(int pagina){
    int calc = 0;
    calc += sizeof(Flag);
    calc += pagina * TAMANHO_PAGINA; //offset sem flag

    return (calc);
}

//ESCREVER NO ARQUIVO DE DADOS
int getStrSize(tRegistro *reg, char *buffer){
    stringstream tmpBuffer;
    tmpBuffer << reg->id << "|" << reg->titulo << "|" << reg->genero << "|";
    string tmpString = tmpBuffer.str();

    strcpy(buffer, tmpString.c_str());
#ifdef DEBUG
    //cout << buffer << ": " << strlen(buffer) << endl; 
#endif
    //sprintf(buffer, "%d|%s|%s|", reg->id, reg->titulo, reg->genero);
    return strlen(buffer);
}

int arquivo_escrever(tRegistro *reg){
    FILE *fp;
    fp = fopen(DATA_FILE, "ab"); // abre no final

    char buffer[300];
    int offset;
    offset = ftell(fp);
    
    //escrita no arquivo
    int tamanho = getStrSize(reg, buffer); //Coloca escrito no buffer e retorna tamanho
    fwrite(&tamanho, sizeof(tamanho), 1, fp);
    fwrite(buffer, tamanho, 1, fp);
    fclose(fp);
    return offset; //retorna offset de onde foi escrito
}   

//LER DO ARQUIVO DE DADOS
char *parser(char *buffer, int *pos) {
    int posi = *pos;

    while(buffer[*pos]!='|')
        (*pos)++;
    buffer[*pos] = '\0';
    (*pos)++;
    return &buffer[posi];
}

tRegistro arquivo_ler(Arvore *arv, FILE *fp, int *offset){
    int tamanho, pos;
    char buffer[300];
    tRegistro reg;

    (*offset) = ftell(fp);
#ifdef DEBUG
    cout << "Offset lido: " << *offset << endl;
#endif

    fread(&tamanho, sizeof(tamanho), 1, fp);
    if(feof(fp)){
#ifdef DEBUG
        cout << "Chegou no final do arquivo" << endl << endl;
#endif
        (*offset) = -1;
        return reg;
    }

    fread(buffer, tamanho, 1, fp);
    pos = 0;
    sscanf(parser(buffer, &pos), "%d", &reg.id);
    string tituloTmp(parser(buffer, &pos));
    string generoTmp(parser(buffer, &pos));

    reg.titulo = tituloTmp;
    reg.genero = generoTmp;

#ifdef DEBUG
    cout << "Leu id: " << reg.id << ", titulo: " << reg.titulo << ", genero: " << reg.genero << endl;
#endif

    return reg;
}

void insercao(Arvore *arv, int tmpId, string title, string gender, Flag *fflag){
    fflag->raiz = arv->raiz;
    setFlagFalse(arv, fflag);

    tRegistro tmpReg;
    entrada elemEntrada;
    
    //Atribuindo valores ao registro
    tmpReg.id = tmpId;
    tmpReg.titulo = title;
    tmpReg.genero = gender;

    if(arvore_busca(arv, tmpId) != -1){
        clog << "Chave " << tmpId << " duplicada" << endl;
#ifdef DEBUG
        cout << "Insercao duplicada" << endl;
#endif
        return;
    }

    int offset = arquivo_escrever(&tmpReg);
    elemEntrada.id = tmpId;
    elemEntrada.byte_offset = offset; // ajeitar (?)
    arvore_inserir(arv, elemEntrada);
    setFlagTrue(arv, fflag);
}

void busca(Arvore *arv, int idBusca, FILE *fp, Flag *fflag){
    setFlagFalse(arv, fflag);
    int offset = arvore_busca(arv, idBusca);
#ifdef DEBUG
    cout << "Offset: " << offset << endl;
#endif
    if(offset == -1){
        clog << "Chave " << idBusca << " nao encontrada" << endl;
        return;
    }else{
        rewind(fp);
        fseek(fp, offset, SEEK_SET);
        int backupOffset = offset;
        tRegistro tmpReg;
        tmpReg = arquivo_ler(arv, fp, &offset);

        clog << "Chave " << tmpReg.id << " encontrada, ";
        clog << "offset " << backupOffset;
        clog << ", Titulo: " << tmpReg.titulo;
        clog << ", Genero: " << tmpReg.genero << endl;
    }
    setFlagTrue(arv, fflag);
}

void arvore_imprimir(Arvore *arv, Flag *fflag){
    setFlagFalse(arv, fflag);
    Pagina pag;
    int i, j;

    int *fila = new int(1000); //MAXIMO de nos = 1000;
    int filafim = 0, curr=0;
    int *nivel = new int(1000);
    nivel[0] = 0;

    clog << "Execucao de operacao para mostrar a arvore-B gerada:" << endl;

#ifdef DEBUG
    printf("--------------------\n");
    printf("Raiz: %d\n", arv->raiz);
    printf("Imprimindo %d paginas\n", arv->paginas);
#endif

    fila[filafim++] = arv->raiz;
    while(curr != filafim){
#ifdef DEBUG
        cout << "Pagina" << fila[curr] << endl;
#endif
        pagina_ler(arv, &pag, fila[curr]);
        clog << nivel[curr] << " " << pag.num_chaves;
        for(i = 0; i < pag.num_chaves; i++) {
            clog << " <" << pag.entradas[i].id << "/" << pag.entradas[i].byte_offset << ">"; 
#ifdef DEBUG
            printf("ponteiro[%d]: = %d\n", i, pag.ponteiros[i]);
            printf("elemento[%d] = %d:%d\n", i, pag.entradas[i].id, pag.entradas[i].byte_offset); //ajeitar(?)
#endif
            if(pag.ponteiros[i] != -1){
                nivel[filafim] = nivel[curr] + 1;
                fila[filafim++] = pag.ponteiros[i];
            }
        }
        if(pag.ponteiros[i] != -1){
                nivel[filafim] = nivel[curr] + 1;
                fila[filafim++] = pag.ponteiros[i];
        }
        clog << endl;
#ifdef DEBUG
        printf("ponteiro[%d]: = %d\n", i, pag.ponteiros[i]);
        printf("--------------------\n");
#endif
        curr++;
    }
    delete(fila);
    delete(nivel);
    setFlagTrue(arv, fflag);
}

void printMenu(){
    cout << endl;
    cout << "Menu" << endl;
    cout << "1) Criar indice" << endl;
    cout << "2) Inserir Musica" << endl;
    cout << "3) Pesquisar Musica por ID" << endl;
    cout << "4) Remover Musica por ID" << endl;
    cout << "5) Mostrar arvore-B" << endl;
    cout << "6) Fechar o programa" << endl;
#ifdef DEBUG
    cout << "7) Apagar arquivos" << endl;
#endif



    cout << "Digite a opcao desejada: ";
}

void arvore_debug(Arvore *arv){
    cout << endl;
    cout << "Debug arvore" << endl;
    cout << "Paginas " << arv->paginas << endl;
    cout << "Raiz " << arv->raiz << endl;
    cout << "Ponteiro " << arv->ponteiro << endl;
    cout << endl;
}

void pagina_imprimir(Arvore *arv, int idPag){
    Pagina pag;
    cout << endl << "Pagina" << idPag << endl;
    pagina_ler(arv, &pag, idPag);
    for(int i = 0; i < MAXIMO_CHAVES; i++) {
        printf("ponteiro[%d]: = %d\n", i, pag.ponteiros[i]);
        printf("elemento[%d] = %d:%d\n", i, pag.entradas[i].id, pag.entradas[i].byte_offset); //ajeitar(?)
    }
    cout << endl;
}

void setFlagFalse(Arvore *arv, Flag *fflag){
    fflag->raiz = arv->raiz;
#ifdef DEBUG
    cout << "Setou flag falsa" << endl;
#endif
    int offset = ftell(arv->fp);
    fflag->flagOk = false;
    rewind(arv->fp);
    fwrite(fflag, sizeof(Flag), 1, arv->fp); //escreve as infos no arquivo arvore
    
    fseek(arv->fp, offset, SEEK_SET); //volta o ponteiro pra onde estava
}

void setFlagTrue(Arvore *arv, Flag *fflag){
    fflag->raiz = arv->raiz;
    fflag->paginas = arv->paginas;
    fflag->ponteiro = arv->ponteiro;
#ifdef DEBUG
    cout << "Setou flag verdadeira" << endl;
#endif
    int offset = ftell(arv->fp);
    fflag->flagOk = true;
    rewind(arv->fp);
    fwrite(fflag, sizeof(Flag), 1, arv->fp); //escreve as infos no arquivo arvore

    fseek(arv->fp, offset, SEEK_SET); //volta o ponteiro pra onde estava
}

int checkFlag(Arvore *arv, Flag *fflag){
    int offset = ftell(arv->fp);
    rewind(arv->fp);
    fread(fflag, sizeof(Flag), 1, arv->fp);

    
    fseek(arv->fp, offset, SEEK_SET);
    if(fflag->flagOk == false)
        return 1;
    else
        return 0;
}