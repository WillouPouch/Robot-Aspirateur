#define SIZE 10

/*--------------------------------------------
Structure d'un noeud lors de l'exploration
--------------------------------------------*/
typedef struct Noeud Noeud;
struct Noeud {
	char action;
	int nodeX;
	int nodeY;
	struct Noeud* parent;
	bool visited;
};


/*--------------------------------------------
Classe de l'agent aspirateur
--------------------------------------------*/
class AgentAspirateur {

private:

	// BDI
	char** beliefs;
	char** desire;
	char* intentions;

	//Appel à l'environnement
	Environnement env;

	//Position du robot
	int position_x, position_y;

public:

	//Construteur et destructeur
	AgentAspirateur();
	~AgentAspirateur(){}

	//Cycle de vie
	char** Observe(char** beliefs_temp, char **manoir);
	void updateBeliefs(char** beliefs_temp, char **manoir);
	bool testDesir(); 
	char* choixAction(bool testCarteVide); 
	void Agir(char action, char** beliefs_temp);
	void afficheBeliefs();

	//Algorithme d'exploration: Breadth-First Search
	void ExploNonInformee(char **beliefs, int posX, int posY);

	//Algorithme d'exploration avec heuristique
	void ExploInformee(char **beliefs, int posX, int posY);
	int distanceManhanttan(int xA, int xB, int yA, int yB);

	//Actions
	void Gauche(char** beliefs_temp);
	void Droite(char** beliefs_temp);
	void Haut(char** beliefs_temp);
	void Bas(char** beliefs_temp);
	void Aspirer(char** beliefs_temp);
	void RamasserBijou(char** beliefs_temp);
	void neRienFaire(char** beliefs_temp);
};