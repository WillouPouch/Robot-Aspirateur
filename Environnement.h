#define SIZE 10

/*--------------------------------------------
Classe de l'environnement
--------------------------------------------*/
class Environnement {

private:

	//Carte du monde contenant poussi�re et bijoux
	char** carte;

public:

	//Construteur et destructeur
	Environnement();
	~Environnement(){}

	//Initialisation de l'environnement
	void initialiser();

	//Getters et setters
	void updateCarte();
	char** getCarte();

	//G�n�ration de la poussi�re et des bijoux
	void generateDirt();
	void generateJewel();

	//Test de carte remplie
	bool mapNotFullDirt();
	bool mapNotFullJewel();

	//Temp
	void afficheCarte();
};