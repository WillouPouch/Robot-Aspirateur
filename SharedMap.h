#define SIZE 10

/*--------------------------------------------
Classe de l'interface graphique
--------------------------------------------*/
class SharedMap {

private:

public:

	//Construteur et destructeur
	SharedMap() {
		manoir = new char*[SIZE];

		for (int i = 0; i < SIZE; i++) {
			manoir[i] = new char[SIZE];
			for (int j = 0; j < SIZE; j++) {
				manoir[i][j] = ' ';
			}
		}
	}
	~SharedMap() {}

	char** manoir;
};

