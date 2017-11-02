// main.cpp : Définit le point d'entrée pour l'application console.

#include "stdafx.h"

//Mutex
mutex mut;

SharedMap *sharedData = new SharedMap();

/*--------------------------------------------
Update du Thread environnement
--------------------------------------------*/
void updateEnv(Environnement* carte) {

	//Cycle de vie de l'environnement
	while (1) {
		if (carte->mapNotFullDirt()) {
			carte->generateDirt();
		}
		if (carte->mapNotFullJewel()) {
			carte->generateJewel();
		}
	
		sharedData->manoir = carte->getCarte();
		//mut.lock();
		//mut.unlock();

		//carte->afficheCarte();
		/*int quitter = 0;
		cout << "Quitter ? \n Oui = 1, Non = 0 \n Choix: ";
		cin >> quitter;
		cout << "\n\n\n";
		if (quitter == 1) {
			break;
		}*/
		/*std::this_thread::sleep_for(std::chrono::milliseconds(400));
		system("cls");*/
	}
}


/*--------------------------------------------
Update du Thread agent
--------------------------------------------*/
void updateAgent(AgentAspirateur* aspi2000) {
	
	//Variables locales
	bool carteVide = false;
	char* intentions;
	char** beliefs_temp = new char*[SIZE];

	//Allocation d'espace pour la carte des beliefs temporaires
	for (int i = 0; i < SIZE; i++) {
		beliefs_temp[i] = new char[SIZE];
		for (int j = 0; j < SIZE; j++) {
			beliefs_temp[i][j] = ' ';
		}
	}

	//Cycle de vie de l'agent
	while (1) {
		//Observation de l'environnement
		beliefs_temp = aspi2000->Observe(beliefs_temp, sharedData->manoir);

		//Mise à jour des beliefs
		aspi2000->updateBeliefs(beliefs_temp, sharedData->manoir);

		//Affichage des beliefs en console
		/*aspi2000->afficheBeliefs();
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		system("cls");
*/
		//Test de but de l'agent (comparaison à une carte vide)
		carteVide = aspi2000->testDesir();

		//Choix d'une solution via une exploration
		intentions = aspi2000->choixAction(carteVide);

		//Exécution des actions de la solution trouvée
		for (int i = 0; i < sizeof(intentions); i++) {
			//Exécution d'une action
			aspi2000->Agir(intentions[i], beliefs_temp); 

			//Mise à jour des beliefs
			mut.lock();
			aspi2000->updateBeliefs(beliefs_temp, sharedData->manoir);
			mut.unlock();

		//	//Affichage des beliefs
		//	aspi2000->afficheBeliefs(); 

		//	/*int quitter = 0;
		//	cout << "Quitter ? \n Oui = 1, Non = 0 \n Choix: ";
		//	cin >> quitter;
		//	cout << "\n\n\n";
		//	if (quitter == 1) {
		//		break;
		//	}*/
		//	std::this_thread::sleep_for(std::chrono::milliseconds(400));
		//	system("cls");
		}
		//mut.lock();
		//mut.unlock();
	}
}


/*--------------------------------------------
Update du Thread interface graphique
--------------------------------------------*/
void updateFenetre(InterfaceGraphique *fenetre) {
	while (1) {
		fenetre->afficheCarte(sharedData->manoir);
		//mut.lock();
		//mut.unlock();

		int quitter = 0;
		cout << "Quitter ? \n Oui = 1, Non = 0 \n Choix: ";
		cin >> quitter;
		cout << "\n\n\n";
		if (quitter == 1) {
			break;
		}
	}
	
}


/*--------------------------------------------
Main
--------------------------------------------*/
int main() {
	//Objets
	SharedMap *sharedMap = new SharedMap();
	Environnement *map = new Environnement();
	AgentAspirateur *aspi2000 = new AgentAspirateur();
	InterfaceGraphique *fenetre = new InterfaceGraphique();

	//Threads
	thread environnement(updateEnv, map);
	thread agent(updateAgent, aspi2000);
	thread interface(updateFenetre, fenetre);

	//Synchronisation des threads
	environnement.join();
	agent.join();
	interface.join();

    return 0;
}

