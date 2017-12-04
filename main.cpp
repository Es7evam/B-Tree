#include "bTree.hpp"

int main(void) {
    freopen (LOG_FILE,"a",stderr);

    int offset = 0;
    char buff[1000] = {};
    FILE *dataFile = NULL;
    FILE *consultas = NULL;

    tRegistro temp;
    Arvore arvore;
    Flag okFlag;

    bool dadosExiste = true;
    dataFile = fopen(DATA_FILE, "rb+"); //ajeitar nome arquivo
    if(dataFile == NULL){
        cout << "Nao existia arquivo de dados, foi criado um vazio" << endl;
        dataFile = fopen(DATA_FILE, "wb+");
        dadosExiste = false;
    }

    
    arvore_iniciar(&arvore, false, dataFile, &okFlag); //não constrói, só inicia
    //ajeitar
    arvore_debug(&arvore);
    int  idTmp;
    string tituloTmp, generoTmp, strNull;
    bool stay = true;
    while(stay){
        printMenu();
        int opt;
        cin >> opt;
#ifdef DEBUG
        cout << endl << endl;
#endif
        switch(opt){
           case 1: 
                if(!dadosExiste){
                    cout << "Arquivo de dados inexistente!" << endl;
                    cout << "Impossivel construir indice..." << endl;

                    break;
                }
                clog << "Execucao da criacao do arquivo de indice " << INDEX_FILE;
                clog << " com base no arquivo de dados " << DATA_FILE << ".\n";
                arvore_iniciar(&arvore, true, dataFile, &okFlag);
                break;
    
           case 2:
                cout << "Digite o id da música " << endl;
                cin >> idTmp;
                while(cin.fail()) {
                    cout << "Digite um valor inteiro!" << endl;
                    cin.clear();
                    std::cin.ignore(256,'\n');
                    cin >> idTmp;
                }

                getchar(); //pega o \n após o id
                cout << "Digite o titulo da musica" << endl;
                getline(cin, tituloTmp);

                cout << "Digite o genero da musica" << endl;
                getline(cin, generoTmp);
                clog << "Execucao de operacao de INSERCAO de " << idTmp << ", " << tituloTmp;
                clog << ", " << generoTmp << "." << endl; //leitura ok

                insercao(&arvore, idTmp, tituloTmp, generoTmp);
                break;
            
            case 3:
                cout << "Digite o id da musica que quer buscar: ";
                cin >> idTmp;
                clog << "Execucao de operacao de PESQUISA de " << idTmp << "." << endl;
                busca(&arvore, idTmp, dataFile);
                break;
            
            case 4:
                cout << "Ainda não implementada :(" << endl << endl;
                //a implementar
                break;
            
            case 5:
                arvore_imprimir(&arvore);
                break;
            
            case 6:
                stay = false;
                break;

#ifdef DEBUG
            case 7:
                fclose(arvore.fp);
                fclose(dataFile);
                arvore.fp = fopen(INDEX_FILE, "wb+");
                dataFile = fopen(DATA_FILE, "wb+");
                arvore_iniciar(&arvore, true, dataFile, &okFlag);
                break;
#endif

            default:
                cout << "Digite um valor valido" << endl;
                break;
        }
        fflush(arvore.fp);
        fflush(dataFile);
    }
#ifdef DEBUG
    //arvore_imprimir(&arvore);
#endif

/*
    //MONTANDO ARVORE
    while(!feof(dataFile)) { // 0 se final do arquivo
        offset = ftell(dataFile);
        int ids;
        string title;
        string gen;

        fgets(buff, 1000, dataFile);
        
        memcpy(temp.CEP, buff, 8);
        
        //temp.CEP[8] = '\0';
        //AJEITAR

        temp.id = ids;
        temp.titulo = title;
        temp.genero = gen;
        temp.byte_offset = offset; // Pega o byte offset, adquirido anteriormente.
        arvore_inserir(&arvore, temp);
    }
*/
/*
    consultas = fopen("consultas.txt", "r");
    while(!feof(consultas)) {
    	fgets(buff, 1000, consultas);
        LIMPAR_QUEBRA(buff);
 
		
        //offset = arvore_busca(&arvore, buff);
         
        // AJEITAR AQUI
        
        if(offset < 0) {
            printf("%s NAO ENCONTRADO\n", buff);
        }
        else {
            fseek(dataFile, offset, SEEK_SET);
            fgets(buff, 1000, dataFile);
            LIMPAR_QUEBRA(buff);
            printf("%s\n", buff);
        }
    }
*/
    fclose(arvore.fp);
    fclose(dataFile);
    return 0;
}
