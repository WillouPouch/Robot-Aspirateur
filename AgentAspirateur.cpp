#include "stdafx.h"

/*--------------------------------------------
Constructeur de l'agent aspirateur
--------------------------------------------*/
AgentAspirateur::AgentAspirateur() {
	//initialisation du robot en (0,0)
	position_x = 0;
	position_y = 0;

	//initialisation du désir = carte vide
	desire = new char*[SIZE];
	for (int i = 0; i < SIZE; i++) {
		desire[i] = new char[SIZE];
		for (int j = 0; j < SIZE; j++) {
			desire[i][j] = ' ';
		}
	}
}


/*--------------------------------------------
Observation de l'environnement

	Entrée: Copie du tableau des beliefs
	Sortie: Copie du tableau des beliefs
--------------------------------------------*/
char** AgentAspirateur::Observe(char** beliefs_temp, char **manoir) {
	/*beliefs_temp = manoir;
	beliefs_temp[position_y][position_x] = 'R';
	return beliefs_temp;*/
	beliefs = manoir;
	beliefs[position_y][position_x] = 'R';
	return beliefs;
}


/*--------------------------------------------
Mise à jour des croyances

	Entrée: Copie du tableau des beliefs
--------------------------------------------*/
void AgentAspirateur::updateBeliefs(char** beliefs_temp, char **manoir) {
	manoir = beliefs_temp;
}


/*--------------------------------------------
Test du désir: on compare la carte des beliefs
à la carte des désirs (carte vide)
--------------------------------------------*/
bool AgentAspirateur::testDesir() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			//On teste que chaque case du belief est vide, excepté la case du robot
			if ((beliefs[i][j] != desire[i][j]) && beliefs[i][j] != 'R') return false;
		}
	}
	return true;
}


/*--------------------------------------------
Choix de l'action
	
	Entrée: booléen de test du désir
	Sortie: Tableau d'intentions du robot
--------------------------------------------*/
char* AgentAspirateur::choixAction(bool testCarteVide) {
	//Si la carte est vide, le robot ne fait rien
	if (testCarteVide) {
		intentions = new char[1];
		intentions = "W";
	}
	//Appel de l'algorithme d'exploration
	else {
		ExploNonInformee(beliefs, position_x, position_y);
		//ExploInformee(beliefs, position_x, position_y);
	}

	return intentions;
}


/*--------------------------------------------
Exploration Breadth-First Search

	Entrées: carte des beliefs du robot
			 position X du robot
			 position Y du robot
	But: Affecter une liste d'intentions 
		 au robot
--------------------------------------------*/
void AgentAspirateur::ExploNonInformee(char **beliefs, int posX, int posY) {

	//Noeud courant et son pointeur
	Noeud currentNode;
	Noeud *pNode = &currentNode;

	//FIFO des noeuds
	queue<Noeud*> Queue;

	//Stockage de la solution
	char *chemin;

	//Compteurs
	int compteur = 0;
	int length = 0;
	
	//Initialisation du noeud racine
	pNode->action = 'R';
	pNode->nodeX = posX;
	pNode->nodeY = posY;
	pNode->parent = NULL;
	pNode->visited = false;
	
	//Insertion du noeud racine dans la queue
	Queue.push(pNode);
	
	//Tant que la file n'est pas vide
	while (!Queue.empty()) {

		//Récupération du noeud courant
		pNode = Queue.front();

		posY = pNode->nodeY;
		posX = pNode->nodeX;

		//Destruction du noeud courant dans la queue
		Queue.pop();

		if (!pNode->visited) {

			//Condition de réussite : le robot est sur une case non vide
			if (beliefs[posY][posX] != ' ' && beliefs[posY][posX] != 'R') {

				chemin = new char[length];

				//Action 'Aspirer' si poussière
				if (beliefs[posY][posX] == 'D') {
					chemin[compteur] = 'A';
					compteur++;
				}
				//Action 'Ramasser' si bijou
				else if (beliefs[posY][posX] == 'J') {
					chemin[compteur] = 'R';
					compteur++;
				}
				//Action 'Ramasser' puis 'Aspirer' si poussière et bijou
				else if (beliefs[posY][posX] == 'B') {
					chemin[compteur] = 'A';
					compteur++;
					chemin[compteur] = 'R';
					compteur++;
				}

				//Parcours des noeuds en fonction des parents
				while (pNode->parent != NULL) {
					chemin[compteur] = pNode->action;
					compteur++;
					pNode = pNode->parent;
				}
			
				chemin[compteur] = { '\0' }; //Troncature de l'espace alloué

				intentions = new char[compteur];
				int i, j;
				
				//Inversion de l'ordre des actions à réaliser
				//Affectation des actions aux intentions du robot
				for (i = 0, j = compteur-1; i < compteur; i++, j--) {
					intentions[i] = chemin[j];
				}

				intentions[compteur] = { '\0' }; //Troncature de l'espace alloué
				return;
			}

			//--TEMP-- if (length == 0 || (pNode->nodeX == pNode->parent->nodeX && pNode->nodeY == pNode->parent->nodeY) == false) {

			//Case voisine: Haut (si dans la carte)
			if (posY - 1 >= 0) {
				int posHaut = posY - 1;
				Queue.push(new Noeud{ 'H', posX, posHaut, pNode, false });				
			}
			//Case voisine: Bas (si dans la carte)
			if (posY + 1 < SIZE) {
				int posBas = posY + 1;
				Queue.push(new Noeud{ 'B', posX, posBas, pNode, false });
			}
			//Case voisine: Droite (si dans la carte)
			if (posX + 1 < SIZE) {
				int posDroite = posX + 1;
				Queue.push(new Noeud{ 'D', posDroite, posY, pNode, false });
			}
			//Case voisine: Gauche (si dans la carte)
			if (posX - 1 >= 0) {
				int posGauche = posX - 1;
				Queue.push(new Noeud{ 'G', posGauche, posY,pNode, false });
			}

			pNode->visited = true;
		}
		length++;
	}

}


/*--------------------------------------------
Exploration Informée Best-First Search avec
	heuristique distance de Manhanttan

	Entrées: carte des beliefs du robot
			 position X du robot
			 position Y du robot
	But: Affecter une liste d'intentions
		 au robot
--------------------------------------------*/
void AgentAspirateur::ExploInformee(char ** beliefs, int posX, int posY) {
	//Noeud courant et son pointeur
	Noeud currentNode;
	Noeud *pNode = &currentNode;

	//Noeud choisi
	Noeud goalNode;
	Noeud *pGoal = &goalNode;

	//Stockage de la solution
	char *chemin;

	//Distance de Manhattan: dM(A,B) = abs(xB - xA) + abs(yB - yA)
	int dM = 0;
	int minDM;
	bool firstLoop = true;
	bool firstLoopGoal = true;
	int dMtoGoal, dMtoGoalMin;
	char voisin;

	//Initialisation du noeud racine
	pNode->action = 'R';
	pNode->nodeX = posX;
	pNode->nodeY = posY;
	pNode->parent = NULL;
	pNode->visited = false;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (beliefs[i][j] != ' ' && beliefs[i][j] != 'R') {
				dM = distanceManhanttan(pNode->nodeX, pNode->nodeY, i, j);
				if (firstLoop) {
					minDM = dM;
					pGoal->action = beliefs[i][j];
					pNode->nodeX = j;
					pNode->nodeY = i;
					firstLoop = false;
				} 
				else if (dM < minDM) {
					minDM = dM;
					pGoal->action = beliefs[i][j];
					pNode->nodeX = j;
					pNode->nodeY = i;
				}
			}
		}
	}
	cout << minDM << " - " << pGoal->action << ": (" << pNode->nodeX << ", " << pNode->nodeY << ") \n";
	chemin = new char[minDM];

	for (int i = 0; i < minDM; i++) {

		//Case voisine: Haut (si dans la carte)
		if (pNode->nodeY - 1 >= 0) {
			int posHaut = posY - 1;
			dMtoGoal = distanceManhanttan(pNode->nodeX, posHaut, pGoal->nodeX, pGoal->nodeY);
			if (firstLoopGoal) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'H';
				firstLoopGoal = false;
			}
			else if (dMtoGoal < dMtoGoalMin) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'H';
			}
		}
		//Case voisine: Bas (si dans la carte)
		if (pNode->nodeY + 1 < SIZE) {
			int posBas = posY + 1;
			dMtoGoal = distanceManhanttan(pNode->nodeX, posBas, pGoal->nodeX, pGoal->nodeY);
			if (firstLoopGoal) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'B';
				firstLoopGoal = false;
			}
			else if (dMtoGoal < dMtoGoalMin) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'B';
			}
		}
		//Case voisine: Droite (si dans la carte)
		if (pNode->nodeX + 1 < SIZE) {
			int posDroite = posX + 1;
			dMtoGoal = distanceManhanttan(posDroite, pNode->nodeY, pGoal->nodeX, pGoal->nodeY);
			if (firstLoopGoal) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'D';
				firstLoopGoal = false;
			}
			else if (dMtoGoal < dMtoGoalMin) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'D';
			}
		}
		//Case voisine: Gauche (si dans la carte)
		if (pNode->nodeX - 1 >= 0) {
			int posGauche = posX - 1;
			dMtoGoal = distanceManhanttan(posGauche, pNode->nodeY, pGoal->nodeX, pGoal->nodeY);
			if (firstLoopGoal) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'G';
				firstLoopGoal = false;
			}
			else if (dMtoGoal < dMtoGoalMin) {
				dMtoGoalMin = dMtoGoal;
				voisin = 'G';
			}
		}
		cout << "Goal min: " << dMtoGoalMin << "\n";
		cout << "Voisin: " << voisin << "\n";

		switch (voisin) {
			case 'D':
				pNode->nodeX++;
				pNode->action = 'D';
				break;
			case 'G':
				pNode->nodeX--;
				pNode->action = 'G';
				break;
			case 'H':
				pNode->nodeY--;
				pNode->action = 'H';
				break; 
			case 'B':
				pNode->nodeY++;
				pNode->action = 'B';
				break;
		}
		chemin[i] = pNode->action;
	}
	chemin[minDM] = { '\0' };
	cout << "Wallah chemin: " << chemin << "\n";

}


/*--------------------------------------------
Calcul de la distance de Manhanttan

	Entrées: Coordonnées des 2 éléments
	Sortie: Distance de Manhattan entre 
			les 2 éléments
--------------------------------------------*/
int AgentAspirateur::distanceManhanttan(int xA, int xB, int yA, int yB) {
	return abs(xB - xA) + abs(yB - yA);
}


/*--------------------------------------------
Choix de l'action de l'agent

	Entrées: Char correspondant à une action
			 Copie du tableau des beliefs
--------------------------------------------*/
void AgentAspirateur::Agir(char action, char** beliefs_temp) {
	switch (action) {
		case 'H':
			Haut(beliefs_temp);
			break;
		case 'B':
			Bas(beliefs_temp);
			break;
		case 'G':
			Gauche(beliefs_temp);
			break;
		case 'D':
			Droite(beliefs_temp);
			break;
		case 'A':
			Aspirer(beliefs_temp);
			break;
		case 'R':
			RamasserBijou(beliefs_temp);
			break;
		default:
			neRienFaire(beliefs_temp);
			break;
	}
}


/*--------------------------------------------
Affichage des beliefs de l'agent
--------------------------------------------*/
void AgentAspirateur::afficheBeliefs() {
	cout << "Beliefs du robot aspirateur: \n\n";

	for (int i = 0; i < SIZE; i++) {
		cout << "-----------------------------------------";
		cout << "\n";
		for (int j = 0; j < SIZE; j++) {
			cout << "| " << beliefs[i][j] << " ";
			if (j == SIZE - 1) cout << "|";
		}
		cout << "\n";
		if (i == SIZE - 1) cout << "-----------------------------------------";
	}

	cout << "\n\n";

	//clear la console
	//std::this_thread::sleep_for(std::chrono::milliseconds(400));
	//system("cls");
}


/*--------------------------------------------
Action gauche

	Entrée: Copie du tableau des beliefs
--------------------------------------------*/
void AgentAspirateur::Gauche(char** beliefs_temp) {
	beliefs_temp[position_y][position_x--] = ' ';
	beliefs_temp[position_y][position_x] = 'R';
}


/*--------------------------------------------
Action droite

	Entrée: Copie du tableau des beliefs
--------------------------------------------*/
void AgentAspirateur::Droite(char** beliefs_temp) {
	beliefs_temp[position_y][position_x++] = ' ';
	beliefs_temp[position_y][position_x] = 'R';
}


/*--------------------------------------------
Action haut

	Entrée: Copie du tableau des beliefs
--------------------------------------------*/
void AgentAspirateur::Haut(char** beliefs_temp) {
	beliefs_temp[position_y--][position_x] = ' ';
	beliefs_temp[position_y][position_x] = 'R';
}


/*--------------------------------------------
Action bas

	Entrée: Copie du tableau des beliefs
--------------------------------------------*/
void AgentAspirateur::Bas(char** beliefs_temp) {
	beliefs_temp[position_y++][position_x] = ' ';
	beliefs_temp[position_y][position_x] = 'R';
}


/*--------------------------------------------
Action aspirer poussière
--------------------------------------------*/
void AgentAspirateur::Aspirer(char** beliefs_temp) {
	
}


/*--------------------------------------------
Action ramasser bijou
--------------------------------------------*/
void AgentAspirateur::RamasserBijou(char** beliefs_temp) {
	
}


/*--------------------------------------------
Action ne rien faire
--------------------------------------------*/
void AgentAspirateur::neRienFaire(char** beliefs_temp) {
}
