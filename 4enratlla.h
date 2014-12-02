/*! \file 4enratlla.h
 * \brief Capçalera per 4enratlla.c
 * \author Oscar Saleta Reig
 */
#ifndef _4ENRATLLA_
#define _4ENRATLLA_

Node *inicialitzaJoc (); //assigna memòria a un node amb tauler buit (per començar el joc)
void helloHuman(int,int); //greeting on s'informa al jugador que va amb les X i es mostra la puntuació
int aplicaTirada(char tauler[N][N], int num_tirada, char jugador); //aplica una tirada del jugador a un tauler
int busca4(char tauler[N][N], char jugador); //busca 4 en ratlla
int buscaFiles(char tauler[N][N], char jugador); //busca 4 en ratlla per files
int buscaColumnes(char tauler[N][N], char jugador); //busca 4 en ratlla per columnes
int buscaDiagonals(char tauler[N][N], char jugador); //busca 4 en ratlla per diagonals

char ferPartida(Node *,int,int,int); //tota una ronda de 4 en ratlla, incrementa la puntuació del guanyador

#endif // _4ENRATLLA_
