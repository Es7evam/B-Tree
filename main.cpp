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

    
    arvore_iniciar(&arvore, false, dataFile, &okFlag); //nao constrói, só inicia
    //ajeitar
#ifdef DEBUG
    arvore_debug(&arvore);
#endif
    int  idTmp;
    string tituloTmp, generoTmp, strNull;
    bool stay = true;
    if(checkFlag(&arvore, &okFlag)){
        arvore_iniciar(&arvore, true, dataFile, &okFlag);
#ifdef DEBUG
    cout << "Arquivo corrompido, recriando-o" << endl;
#endif
    }
    while(stay){

        setFlagFalse(&arvore, &okFlag);
        printMenu();
        int opt;
        cin >> opt; 
        while(cin.fail()) { //validaçao dos dados
            cout << "Digite um valor inteiro!" << endl;
            cin.clear();
            std::cin.ignore(256,'\n');
            cin >> idTmp;
        }
#ifdef DEBUG
        cout << endl << endl;
#endif
        switch(opt){
           case 1: 
                clog << "Execucao da criacao do arquivo de indice " << INDEX_FILE;
                clog << " com base no arquivo de dados " << DATA_FILE << ".\n";
                arvore_iniciar(&arvore, true, dataFile, &okFlag);
                break;
    
           case 2:
                cout << "Digite o id da musica " << endl;
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

                insercao(&arvore, idTmp, tituloTmp, generoTmp, &okFlag);
                break;
            
            case 3:
                cout << "Digite o id da musica que quer buscar: ";
                cin >> idTmp;
                clog << "Execucao de operacao de PESQUISA de " << idTmp << "." << endl;
                busca(&arvore, idTmp, dataFile, &okFlag);
                break;
            
            case 4:
                cout << "Ainda nao implementada :(" << endl << endl;
                //a implementar
                break;
            
            case 5:
                arvore_imprimir(&arvore, &okFlag);
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
        setFlagTrue(&arvore, &okFlag);
    }
    fclose(arvore.fp);
    fclose(dataFile);
    return 0;
}
