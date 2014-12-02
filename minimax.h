/*! \file minimax.h 
 * \brief Capçalera per minimax.c
 * \author Oscar Saleta Reig
 */
#ifndef _MINIMAX_
#define _MINIMAX_

#define N 8 ///< El tauler és NxN
#define BUIT ' ' ///< Un espai no ocupat del tauler
#define HUMA 'X' ///< Un espai ocupat per l'usuari
#define ORDI 'O' ///< Un espai ocupat per l'ordinador

/**
 * Estructura d'un node per formar arbres recursius i aplicar MiniMax
 */
typedef struct node {
    struct node **fills; /**< Punter a punter. Cada fill serà un punter a node, i cada node
                              conté la direcció de memòria dels punters als fills */
    int n_fills; /**< Nombre de fills (nombre de columnes buides) */
    char tauler[N][N]; /**< Tauler del node */
    double valor; /**< Valor heurístic pel tauler d'aquest node */
} Node;

Node *creaNode(char tauler[N][N], int num_tirada, char jugador); //crea 1 node 
void crea1Nivell(Node *, char); //crea els fills d’1 node
void creaArbre(Node *,int); //crea 2 nivells d‘arbre, a partir d’un node
void recorreArbre(Node *,int); //funció que recorre tot l’arbre

void esborraNode(Node *); //esborra 1 node de l’arbre
void esborra1Nivell(Node *); //esborra tots els fills d’1 node, però no aquest
void esborraArbre(Node *,int); //esborra tots els nodes de l’arbre, passant-li l’arrel.

int determinaFills(char tauler[N][N]); //comprova el nº de fills que pot tenir un node
int determinaColumna(char tauler[N][N], int num_tirada); //mira quina és la num_tirada-èssima columna buida
int determinaFila(char tauler[N][N], int columna); //mira quina és la fila buida més baixa de la columna
int tiraHuma(Node *); //llegeix jugada de l'huma i actualitza el node actual de la partida


void copiaTauler(char input[N][N], char output[N][N]); //copia el contingut d'un tauler a un altre array
void printTauler(char tauler[N][N]); //imprimeix un tauler maco

int ferMinimax(Node *,int); //retorna la casella escollida per a la millor jugada

double valoraNode(char tauler[N][N]); //funció heurística que valora un node
int winningCols(char tauler[N][N], char jugador); //valorar columnes per valoraNode
int winningRows(char tauler[N][N], char jugador); //valorar files per valoraNode
int winningDiagonals(char tauler[N][N], char jugador); //valorar diagonals per valoraNode

int taulerPle(char tauler[N][N]); //mirar si el tauler està ple (empat)
char altreJugador(char jugador); //donat un jugador, tornar l'altre

#endif // _MINIMAX_
