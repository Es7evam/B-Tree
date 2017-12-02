#include "bTree.hpp"

int main(void) { 
    long offset = 0;
    char buff[1000] = {};
    FILE *DATA_FILE = NULL;
    FILE *consultas = NULL;
    tRegistro temp;
    Arvore arvore;

    DATA_FILE = fopen("dados.dat", "rb"); //ajeitar nome arquivo
    if(DATA_FILE == NULL){
        cout << "Arquivo de dados inexistente!" << endl;
        cout << "Saindo do programa..." << endl;
        return -1;
    }

    arvore_iniciar(&arvore);
    
    arvore.fp = fopen("indices.dat", "rb+");
    if(arvore.fp == NULL)
        arvore.fp = fopen("indices.dat", "wb+"); //ajeitar nome arquivo (talvez rb+)


    int idBusca;
    cout << "Menu\n";
    cout << "1) Criar Índice\n";
    cout << "2) Inserir Música\n";
    cout << "3) Pesquisar Música por ID\n";
    cout << "4) Remover Música por ID\n";
    cout << "5) Mostrar Árvore-B\n";
    cout << "6) Fechar o programa";
    bool stay = true;
    while(stay){
        int opt;
        cin >> opt;
        switch(opt){
    
           case 1: 
                criar_Indice();
                arvore_iniciar();
                break;
    
           case 2:
                inserir();
                break;
            
            case 3:
                cin >> idBusca;
                busca(Arvore *arv, int idBusca);
                break;
            
            case 4:
                //a implementar
                break;
            
            case 5:
                break;
            
            case 6:
                stay = false;
                break;
        }
    }


    //MONTANDO ARVORE
    while(!feof(DATA_FILE)) { // 0 se final do arquivo
        offset = ftell(DATA_FILE);
        int ids;
        string title;
        string gen;

        fgets(buff, 1000, DATA_FILE);
        
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
    arvore_imprimir(&arvore);
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
            fseek(DATA_FILE, offset, SEEK_SET);
            fgets(buff, 1000, DATA_FILE);
            LIMPAR_QUEBRA(buff);
            printf("%s\n", buff);
        }
    }
    fclose(arvore.fp);
    fclose(consultas);
    fclose(DATA_FILE);
    return 0;
}
