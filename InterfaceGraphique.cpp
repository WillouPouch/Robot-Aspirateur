#include "stdafx.h"

InterfaceGraphique::InterfaceGraphique() {
}

void InterfaceGraphique::afficheCarte(char** carte) {
	cout << "Manoir du robot aspirateur: \n\n";

	for (int i = 0; i < SIZE; i++) {
		cout << "-----------------------------------------";
		cout << "\n";
		for (int j = 0; j < SIZE; j++) {
			cout << "| " << carte[i][j] << " ";
			if (j == SIZE - 1) cout << "|";
		}
		cout << "\n";
		if (i == SIZE - 1) cout << "-----------------------------------------";
	}

	cout << "\n\n";
}
