#include "stdafx.h"

/*--------------------------------------------
Constructeur de l'environnement
--------------------------------------------*/
Environnement::Environnement() {
	initialiser();
}

/*--------------------------------------------
Initialisation de l'environnement
--------------------------------------------*/
void Environnement::initialiser() {
	//carte vide
	carte = new char*[SIZE];

	for (int i = 0; i < SIZE; i++) {
		carte[i] = new char[SIZE];
		for (int j = 0; j < SIZE; j++) {
			carte[i][j] = ' ';
		}
	}

	updateCarte();

	//Pas de poussière ou de bijou sur la case de départ du robot
	carte[0][0] = ' ';
}

/*--------------------------------------------
Update de la carte
--------------------------------------------*/
void Environnement::updateCarte() {
	generateDirt();
	generateJewel();
}


/*--------------------------------------------
Getter de la carte

	Sortie: Carte de l'environnement
--------------------------------------------*/
char** Environnement::getCarte() {
	return carte;
}


/*--------------------------------------------
Générer de la poussière une proba de 1/50
--------------------------------------------*/
void Environnement::generateDirt() {

	srand(time(NULL));
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if(rand() % 50 == 0) {
				if(carte[i][j] == ' ') carte[i][j] = 'D';
				else if(carte[i][j] == 'J') carte[i][j] = 'B';
			}
		}
	}

}


/*--------------------------------------------
Générer d'un bijou une proba de 1/100
--------------------------------------------*/
void Environnement::generateJewel() {

	srand(time(NULL));
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (rand() % 100 == 0) {
				if (carte[i][j] == ' ') carte[i][j] = 'J';
				else if (carte[i][j] == 'D') carte[i][j] = 'B';
			}
		}
	}
}


/*--------------------------------------------
Test si la carte n'est pas déjà remplie de 
poussière
--------------------------------------------*/
bool Environnement::mapNotFullDirt() {
	int nbDirt = 0;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (carte[i][j] == 'D' || carte[i][j] == 'B') nbDirt++;
		}
	}

	if (nbDirt != 100) return true;
	else return false;
}


/*--------------------------------------------
Test si la carte n'est pas déjà remplie de
bijoux
--------------------------------------------*/
bool Environnement::mapNotFullJewel() {
	int nbJewel = 0;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (carte[i][j] == 'J' || carte[i][j] == 'B') nbJewel++;
		}
	}

	if (nbJewel != 100) return true;
	else return false;
}


/*--------------------------------------------
Temp: Affichage de la carte
--------------------------------------------*/
void Environnement::afficheCarte() {
	cout << "Carte du robot aspirateur: \n\n";

	for (int i = 0; i < SIZE; i++) {
		cout << "-----------------------------------------";
		cout << "\n";
		for (int j = 0; j < SIZE; j++) {
			cout << "| " << carte[i][j] << " ";
			if (j == SIZE-1) cout << "|";
		}
		cout << "\n";
		if (i == SIZE-1) cout << "-----------------------------------------";
	}

	cout << "\n\n";
}


