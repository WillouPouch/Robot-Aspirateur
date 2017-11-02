#define SIZE 10

/*--------------------------------------------
Classe de l'environnement
--------------------------------------------*/
class Environnement {

private:

	//Carte du monde contenant poussière et bijoux
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

	//Génération de la poussière et des bijoux
	void generateDirt();
	void generateJewel();

	//Test de carte remplie
	bool mapNotFullDirt();
	bool mapNotFullJewel();

	//Temp
	void afficheCarte();
};