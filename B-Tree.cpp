#include "B-Tree.hpp"

int main(){
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
				inicializar_BTree();
				break;

			case 2:
				inserir();
				break;
			
			case 3:
				busca();
				break;
			
			case 4:
				break;
			
			case 5:
				break;
			
			case 6:
				stay = false;
				break;
		}
	}
    return 0;
}