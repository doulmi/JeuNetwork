#include<stdio.h>
#ifndef _NETWORK_H_
#define _NETWORK_H_

/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Sortie: renvoie 1 si cette forme peux connecter la cellule dessus, si tube >= (1111)2, alors retourne le permier chiffre de tube 
*/
int up( int tube );

/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Sortie: renvoie 1 si cette forme peux connecter droit, si tube >= (1111)2, alors retourne le deuxieme chiffre de tube
 */
int right( int tube );

/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Sortie: renvoie 1 si cette forme peux connecter la cellule dessous, si tube >= (1111)2, alors retourne le troixieme chiffre de tube
 */
int down( int tube );

/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Sortie: renvoie 1 si cette forme peux connecter gauche, si tube > (1111)2, alors retourne le dernier chiffre de tube
 */
int left( int tube );

void setUp( int* tube, int value );
void setRight( int* tube, int value );
void setDown( int* tube, int value );
void setLeft( int* tube, int value );

int upTernary( int need );

int rightTernary( int need );

int downTernary( int need );

int leftTernary( int need );

void setUpTernary( int* need, int up );

void setRightTernary( int* need, int right );

void setDownTernary( int* need, int down );

void setLeftTernary( int* need, int left );

int getRandomBinary();
/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Effet: change la forme en r¨¦gle
 */
void change( int* tube );

/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Effet: change la forme en r¨¦gle en 180 degre
 */
void change180( int* tube );

/*
 * Entr¨¦e: le nombre qui repr¨¦sante une forme
 * Effet: change la forme en r¨¦gle en 270 degre
 */
void change270( int* tube );

/*
 * Entr¨¦e: 
 *		row, col: la position(row, col) dans la matrice du jeu(n,n)
 *		map: la matrice du jeu
 *		fixedTable : une matrice qui sauvegarde tous les positions qui sont d¨¦j¨¤ trait¨¦es
 * Effet: un entier, cette variable represante si cette cellule peut connecter avec les autres
 *				on l'initislise par 80=(2222)3, on v¨¦rifie les quatres directions: dessus, droit, dessous, gauche
 *				par example, si need = (1010)3, c'est ¨¤ dire qu'il faut connecter avec la cellule dessous et la cellule dessus.
 *				si need > 1111, c'est ¨¤ dire, il y a des directions on ne connait pas encore
 *				par example, si need = (2210)3, c'est a dire, on ne sait pas encore s'il faut connecter avec les cellule dessus et la cellule droit, vu qu'ils ne sont pas encore fix¨¦es  
 */
void getNeed( int** map, int i, int j, int n, int** needMap );

/*
 * change (aaaa)2 to (aaaa)3
 */
void binaryToTernary( int* destValue, int srcValue );

/*
 * change (aaaa)3 to (aaaa)2
 */
void ternaryToBinary( int* destValue, int srcValue );

/*
 * Entr¨¦e: a, b
 * Sortie: renvoie vrai si a est un subset de b, sinon renvoie faux. Dans a s¡¯il y a les chiffres 2, on l¡¯ignore, on seulement comparer 0 et 1, ex. 
 */
int isSubset( int a, int b );

/*
 * Entree: tube
 * Sortie: renvoie le numbre de 1 dans le tube( binaire )
 */
int get1Number( int tube );

/*
 * Entr¨¦e: 
 *		map : la matrice du jeu
 *		n : le dimension de la matrice
 *		fixedTable : une matrice qui sauvegarde tous les positions qui sont d¨¦j¨¤ trait¨¦
 * Effet: c'est la fonction pricipale pour r¨¦soudre ce jeu
 */
void getResultHumainLike( int** map, int n  );

/*
 * Entr¨¦e: 
 *		map : la matrice du jeu
 *		n : le dimension de la matrice
 * Effet: montre la matrice dans le console
 */
void displayMap( int** map, int n );

void displayMapInGraph( int** map, int n );

/*
 * Entr¨¦e: 
 *		fixedTable : une matrice qui sauvegarde tous les positions qui sont d¨¦j¨¤ trait¨¦
 *		row, col: la position(row, col) dans la matrice du jeu(n,n)
 *		rest : le nombre de les positions qu'on ne traite pas
 * Effet: on essaye de fixer cette cellule 
 */
void fixTube( int** map, int i, int j, int* rest, int n, int** fixedTable, int** needTable );

void fixTubeGuess( int** map, int row, int col, int* rest, int n, int** fixedTable );

/*
 * Entr¨¦e : le dimension de la matrice
 * Sortie : on cr¨¦er dynamiquement une matrice n * n
 */
int** generateMatrice( int n, int initialVal );

/*
 * Entr¨¦e : le dimension de la matrice
 * on cr¨¦er dynamiquement un graphe du jeu
 */
int** generateGameMap( int n );

/*
 * Entree : 
 *		row, col: la position(row, col) dans la matrice du jeu(n,n)
 *		map: la matrice du jeu
 *		n: dimension du jeu
 * Sortie : generate un tube pour map[ row ][ col ]
 */
int generateSuitableNum( int** map, int row, int col, int n, int** needTable );

/* Entree : 
 *		map: la matrice du jeu
 *		n: dimension du jeu
 * Effet : connecte tous les graphes inconnect¨¦s
 */
void connectAll( int** map, int n );

/*
 * Effet : Connecter tube1 et la cellule dessuse tube2
 */
void connectUp( int* tube1, int* tube2 );

/*
 * Effet : Connecter tube1 et la cellule gauche tube2
 */
void connectLeft( int* tube1, int* tube2 );

void connectRight( int* tube1, int* tube2 );

void connectDown( int* tube1, int* tube2 );

void mask( int* value, int mask );

void changeUnionNumber( int** unionMap, int** map, int row, int col, int destNum, int srcNum );
/* 
 * Entr¨¦e : 
 *		unionMap : stocker les informations du graphe d¨¦connect¨¦, si map[ row ][ col ] partient le ner graphe d¨¦connect¨¦, unionMap[ row ][ col ] = n
 *		unionNum : le nombre du graphe d¨¦connect¨¦ dans map
 *		Effet : Diviser map en different partie s¡¯il est un graphe d¨¦connect¨¦, et ¨¤ la fois ouvrir les boucles ¨¤ l¡¯int¨¦rieur
 */
void preConnectAll( int** map, int n, int** unionMap, int* unionNum );

/* Entree : 
 *		map: la matrice du jeu
 *		row, col: la position(row, col) dans la matrice du jeu(n,n)
 *		unionMap : stocker les informations du graphe d¨¦connect¨¦, si map[ row ][ col ] partient le ner graphe		d¨¦connect¨¦, unionMap[ row ][ col ] = n
 *		unionNum : le nombre du graphe d¨¦connect¨¦ dans map
 * Effet : parcourir un graphe par depth search first et ouvrir les boucles
 */
void destryoCircleDSF( int** map, int row, int col, int** unionMap, int* unionNum );

/*
 * Effet : d¨¦connecte tube1 et la cellule dessuse tube2
 */
void disconnectUp( int* tube1, int* tube2 );

/*
 * Effet : d¨¦connecte tube1 et la cellule droite tube2
 */
void disconnectRight( int* tube1, int* tube2 );

/*
 * Effet : d¨¦connecte tube1 et la cellule dessouse tube2
 */
void disconnectDown( int* tube1, int* tube2 );

/*
 * Effet : d¨¦connecte tube1 et la cellule gauche tube2
 */
void disconnectLeft( int* tube1, int* tube2 );

void shuffleState( int** map, int n );

void getResultByGuess( int** map, int n );

void guess( int** map, int n, int tubeNo, int* finish );

int isFinished( int** map, int n );

void DSF( int ** map, int n, int row, int col, int** unionMap, int* tubeCount );

void guessRestTube( int** map, int** fixedTable, int n, int tubeNo, int* finish );

#endif _NETWORK_H_