/////////////////////////////////////////////
/////////////////////////////////////////////
Organisation des fichiers du projet


Le projet est constitu� des fichiers d�en-t�te suivants:
targetver.h
stdafx.h qui contient toutes les importations de biblioth�ques, les includes du projets et les defines
Environnement.h qui contient la classe Environnement
AgentAspirateur.h qui contient la classe AgentAspirateur et une structure servant � d�finir un noeud (utilis� pendant l�exploration)
InterfaceGraphique.h qui contient la classe InterfaceGraphique permettant de mettre en place l�affichage SDL

Le projet est constitu� des fichiers sources suivants:
stdafx.cpp
AgentAspirateur.cpp qui contient la d�finition des fonctions de la classe AgentAspirateur
Environnement.cpp qui contient la d�finition des fonctions de la classe Environnement
InterfaceGraphique.cpp qui contient la d�finition des fonctions mettant en place l�affichage SDL
main.cpp qui est le point d�entr�e du programme et qui contient la d�finition des fonctions update des threads agent et environnement

Le projet est constitu� des ressources externes suivantes :
bijou.bmp qui est l�image bitmap d�une case avec un bijou
both.bmp qui est l�image bitmap d�une case avec une poussi�re et un bijou
case.bmp qui est l�image bitmap d�une case vide
poussiere.bmp qui est l�image bitmap d�une case avec une poussi�re
robot.bmp qui est l�image bitmap d�une case avec un robot

/////////////////////////////////////////////
/////////////////////////////////////////////
Environnement

/////////////////////////////////////////////
Propri�t�s

L�environnement dans lequel �volue l�agent est :
Compl�tement observable
Stochastique car le prochain �tat de l�environnement ne d�pend pas de son �tat courant
�pisodique car le prochain �pisode ne d�pend pas des actions pr�c�dentes effectu�es
Dynamique car la carte de l�environnement peut changer pendant le cycle de vie de l�agent
Continu car l�environnement est mis � jour en temps r�el
Un seul agent

/////////////////////////////////////////////
Description

L�environnement est une carte quadrill�e compos�e de 10*10 cases comme indiqu� dans l��nonc� du travail. Il est lanc� dans une boucle de vie infinie dans laquelle il g�n�re des poussi�res et des bijoux al�atoirement sur la carte.

/////////////////////////////////////////////
G�n�ration al�atoire des �l�ments sur la carte

Lors de la premi�re g�n�ration de poussi�re et/ou de bijoux sur la carte, l�environnement g�n�re de la poussi�re avec une probabilit� de 1/30 et un bijou avec une probabilit� de 1/50. Au tours suivants, l�environnement v�rifie que la carte n�est pas d�j� pleine puis ajoute des poussi�res avec une probabilit� de 1/200 et des bijoux avec une probabilit� de 1/300. Cette diff�rence entre le premier tour et les tours suivants a �t� mise en place pour que le robot ne soit pas d�bord� par une g�n�ration trop intense de poussi�re et de bijoux.


/////////////////////////////////////////////
/////////////////////////////////////////////
Agent

/////////////////////////////////////////////
Propri�t�s

L�agent aspirateur est un agent bas� sur les buts. Il peut se formuler par un probl�me � simple �tat :
Etat initial : Le robot est positionn� en (0,0) soit dans le coin sup�rieur gauche, et � l��tat � Ne rien faire �.
Op�rateurs : Ne rien faire, Aspirer Poussi�re, Ramasser bijou, Gauche, Droite, Haut, Bas
Fonction de succession {Action, �tat} : 
	{ Droite, Robot d�plac� d�une case � droite }
{ Haut, Robot d�plac� d�une case en haut }
{ Bas, Robot d�plac� d�une case en bas }
{ Gauche, Robot d�plac� d�une case � gauche }
{ Aspirer, Carte beliefs mise � jour }
{ Ramasser, Carte beliefs mise � jour }
{ NeRienFaire, Robot attend }	
Test de but : pas de salet� et pas de bijou au sol
Co�t du chemin : 1 unit� par action

/////////////////////////////////////////////
Fonctionnement de l�agent

Le cycle de vie de l�agent est une boucle infinie qui se d�roule comme suit :
Observation de l�environnement : l�agent lit la carte de l�environnement et l�affecte � ses croyances
Mise � jour de ses croyances : il se place sur la carte
Affichage de ses croyances
Effectue un test de d�sir : compare ses croyances � une carte de l�environnement vide
Choix d�une action (ne fait rien si le d�sir est respect�) : Appel d�un algorithme d�exploration
R�cup�ration des intentions trouv�es
Boucle de parcours des intentions trouv�es
Effectue l�action
Mise � jour de ses croyances
Affichage de ses croyances
Mesure de sa performance

/////////////////////////////////////////////
Mod�lisation des actions

Action �Ramasser Bijou�
Pr�misses : 
	Case contenant un bijou
Robot positionn� sur cette case
Cons�quences :
	Bijou ramass�
	Case vid�e	
Action �Aspirer Poussi�re�
		Pr�misses:
			Case contenant une poussi�re
			Robot positionn� sur cette case
	Cons�quences :
	Poussi�re aspir�e
	Case vid�e
Action �Se d�placer� (Haut, Bas, Gauche, Droite)
		Pr�misses:
			Case suivante sur la carte
		Cons�quences :
	D�placement du robot d�une case
Action �Ne Rien Faire�
		Pr�misses :
			Toutes les cases de la carte sont vides
	Cons�quences :
	Le robot attend

/////////////////////////////////////////////
Mod�lisation de la perception

L�agent peut percevoir son environnement en observant une carte de l�environnement � un moment donn�. Il utilise ensuite ses croyances pour explorer la carte et percevoir les �l�ments tels que la poussi�re et les bijoux.

/////////////////////////////////////////////
BDI

L�agent aspirateur poss�de en tout temps trois attributs: beliefs, desire et intentions. Ces attributs sont impl�ment�s de la mani�re suivante :
Beliefs : l�agent poss�de en tout temps, une carte issue de son observation de l�environnement
Desire : l�agent poss�de une carte de l�environnement vide qu�il utilisera afin d��valuer son travail en le comparant � ses beliefs
Intentions : l�agent poss�de une liste d�actions � effectuer pour arriver � son but


/////////////////////////////////////////////
/////////////////////////////////////////////
Exploration Non-Inform�e

Nous avons choisi de mettre en place une exploration Breadth-First Search (recherche en largeur) pour notre exploration non inform�e.

�tapes de notre algorithme d�exploration non inform�e :

Cr�ation d�un noeud racine contenant la position du robot

Cr�ation d�un file (FIFO) contenant les noeuds voisins

Parcours d�une boucle tant que la file n�est pas vide

A chaque tour, on regarde les cases voisines du noeud courant en gardant en m�moire le noeud parent et on ajoute, si possible, ces noeuds voisins � la file.

Si le noeud courant correspond � une case non vide (? poussi�re ou bijou ou les deux) de la carte, on arr�te le parcours de la file.

Parcours des noeuds parents � partir du noeud solution trouv� afin de cr�er une liste d�actions du robot pour arriver � la solution

Inversion de la liste d�action et envoi aux intentions du robot



/////////////////////////////////////////////
/////////////////////////////////////////////
Exploration Inform�e

Nous avons choisi de mettre en place une exploration Best-First Search pour notre exploration inform�e. Pour cela, nous utilisons la distance de Manhattan pour �valuer le noeud le plus d�sirable. A chaque tour dans l�exploration, on choisit le noeud le plus d�sirable, c�est-�-dire celui avec la distance de Manhattan la plus petite du but et on garde en m�moire une liste d�actions menant aux noeuds d�sirables choisis.

La distance de Manhattan correspond � la distance parcourue entre deux points sur un quadrillage. Elle se calcule avec la formule: M = xA - xB + yA - yB.

�tapes de notre algorithme d�exploration:

Cr�ation d�un noeud racine contenant la position du robot

Parcours de la carte en calculant la distance de Manhattan entre la racine et chaque �l�ment non vide (? poussi�re ou bijou)

Sauvegarde du noeud but (position et contenu) le plus proche et de sa distance 

Parcours des voisins possibles pour arriver au noeud but : on �value quel voisin permet de se rapprocher le plus du noeud but (dans le cas de deux voisins � la m�me distance du but, on prend le premier trouv�) et on ajoute l�action � faire pour l�atteindre dans une liste

Envoi de la liste d�actions aux intentions du robot



/////////////////////////////////////////////
/////////////////////////////////////////////
Mesure de performance

Nous avons �galement impl�ment� une mesure de performance. Comme �nonc� dans les sp�cifications du travail, le robot aspirateur d�pense 1 unit� d��lectricit� � chaque action r�alis�e. Nous avons �galement d�cid� d�attribuer un syst�me de points � l�agent, celui-ci fonctionne de la mani�re suivante :
L�agent gagne un point lorsqu�il ramasse un bijou ou aspire une poussi�re
L�agent perd un point s�il aspire un bijou par erreur

La mesure de performance s�effectue � chaque fois que 10 actions de nettoyage ont �t� r�alis�es par l�agent.

Notions utilis�es:
Rapport de performance = Nombre de points / Quantit� d��lectricit� d�pens�e
Pourcentage d�exploration = Pourcentages des intentions du robot r�alis�es avant de re-�valuer son environnement. Il effectue 100% de ses intentions au d�part.
Progression d�exploration  = Quantit� des actions r�alis�es par le robot avant de re-�valuer son environnement. Il est calcul� en multipliant le pourcentage d�exploration avec le nombre total d�intentions.
Pr�cision de performance = Pourcentage variable permettant d�affiner le rapport de performance. Elle influe sur le pourcentage d�exploration. Elle commence � 10%.

�tapes de la mesure de performance:
On v�rifie que le robot a r�alis� 10 actions de nettoyage
Si c�est le cas, on calcul le rapport de performance
On associe le rapport de performance � un pourcentage d�exploration (100% au d�but) et on l�ajoute dans un tableau qui garde en m�moire les deux derniers rapports de performance
Deux cas se distinguent ensuite :
Le cas o� le rapport est meilleur que le pr�c�dent. Dans ce cas, on diminue le pourcentage d�exploration d�une pr�cision calcul�e (pr�cision de performance)
Le cas o� le rapport est moins bon que le pr�c�dent. Dans ce cas, on augmente le pourcentage d�exploration d�une pr�cision calcul�e (pr�cision de performance)
En fonction du nombre d�actions d�termin�es par l�agent pour arriver au but, on calcule la quantit� d�actions qu�il effectuera en multipliant le nombre d�actions par le pourcentage d�exploration d�termin� plus t�t.

Exemple :
Au d�part, le robot effectue 10 actions de nettoyage � 100%. Il compare son rapport de performance au pr�c�dent et le trouve meilleur. Il baisse donc son pourcentage d�exploration de 10% ; de ce fait, lors de sa prochaine exploration, il effectuera 90% de ses actions. Si la prochaine exploration indique 8 actions � effectuer, il en r�alisera alors 8*0.9  7 avant de r�-observer son environnement.
Cas particuliers :
Si le robot trouve 3 actions ou moins � r�aliser lors de son exploration, m�me s�il trouve un meilleur rapport, il effectuera tout de m�me 100% de ses intentions afin d��viter les boucles
Nous avons �galement mis en place une variable permettant d�am�liorer au fur et � mesure la pr�cision du pourcentage d�exploration, plut�t que de l�augmenter ou de le diminuer de 10% � chaque fois.

Pour l�am�lioration de la pr�cision du pourcentage d�exploration, nous utilisons un bool�en correspondant au sens de parcours du tableau. Ce bool�en change � chaque changement de sens, c�est-�-dire � chaque fois qu�on varie entre rapport meilleur et rapport moins bon. Si le sens a chang�, alors la pr�cision du pourcentage d�exploration sera diminu�e par 2. Ainsi, une progression du pourcentage d�exploration pourrait �tre : 100% - 90%-  80%-  85% - 82,5% - 83,75% - ... .





