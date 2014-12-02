/*! \file 4enratlla.c 
 * \brief Fitxer amb funcions específiques del joc Connecta-4
 * \author Oscar Saleta Reig
 */

#include <stdio.h>
#include <stdlib.h>
#include "minimax.h"
#include "4enratlla.h"

/**
 * Crea un tauler NxN buit
 *
 * \return Un node amb tauler buit
 *
 */
Node *inicialitzaJoc () {
    int i,j;
    Node *p=malloc(sizeof(Node));
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            p->tauler[i][j]=BUIT;
    p->n_fills=N;
    p->fills = malloc(p->n_fills*sizeof(Node *));
    return p;
}


/**
 * Saluda i imprimeix la puntuació
 *
 * \param[in] puntuacio_pc Puntuació del PC
 * \param[in] puntuacio_huma Puntuació de l'usuari
 * 
 * \return Res
 *
 */
void helloHuman (int puntuacio_pc, int puntuacio_huma) {
    system("clear");
    printf("CONNECTA 4.\nJugues amb les X. Sort.\n");
    printf("   Puntuació:\n   - Humà: %d\n   - PC:   %d\n\n",puntuacio_huma, puntuacio_pc);
}


/**
 * Aplica una tirada d'un jugador
 *
 * \param[in,out] tauler Tauler de partida, modificat a la sortida
 * \param[in] num_tirada A quina columna es tira (comença per 0)
 * \param[in] jugador Pot ser HUMA o ORDI
 * 
 * \return -1 si hi ha algun error i no es pot fer la tirada
 * \return  0 si es fa la tirada i no hi ha 4 en ratlla
 * \return  1 si es fa la tirada i hi ha 4 en ratlla
 *
 */
int aplicaTirada (char tauler[N][N], int num_tirada, char jugador) {
    int columna, fila;
    // Busquem la columna buida número num_tirada
    if ( (columna = determinaColumna(tauler,num_tirada)) == -1) {
        fprintf(stderr,"Error: tauler inesperadament ple.\n");
        return -1;
    }
    // Busquem la fila on ha de caure la fitxa
    if ( (fila = determinaFila(tauler,columna)) == -1) {
        fprintf(stderr,"Error: columna %d inesperadament plena.\n",columna);
        return -1;
    }
    tauler[fila][columna] = jugador;
    // Comprovem si el jugador ha guanyat
    return busca4(tauler,jugador);
}


/**
 * Buscar 4 en ratlla del jugador en qüestió
 *
 * \param[in] tauler Estat del tauler després de la jugada
 * \param[in] jugador Jugador que ha posat la fitxa
 *
 * \return 0 si no hi ha 4 en ratlla
 * \return 1 si hi ha 4 en ratlla
 *
 */
int busca4 (char tauler[N][N], char jugador) { 
    if (buscaFiles(tauler,jugador) || buscaColumnes(tauler,jugador) || buscaDiagonals(tauler,jugador))
        return 1;
    return 0;
}


/**
 * Buscar 4 en ratlla del jugador (per files)
 *
 * \param[in] tauler Estat del tauler després de la jugada
 * \param[in] jugador Jugador que ha posat la fitxa
 *
 * \return 0 si no hi ha 4 en ratlla
 * \return 1 si hi ha 4 en ratlla
 *
 */
int buscaFiles(char tauler[N][N], char jugador) {
    int i;
    int count;
    for (i=0; i<N; i++) {
        count=0;
        for (int j=0; j<N; j++) {
            if(tauler[i][j]==jugador) {
                if (++count == 4)
                    return 1;
            } else
                count = 0;
        }
    }
    return 0;
}

/**
 * Buscar 4 en ratlla del jugador (per columnes)
 *
 * \param[in] tauler Estat del tauler després de la jugada
 * \param[in] jugador Jugador que ha posat la fitxa
 *
 * \return 0 si no hi ha 4 en ratlla
 * \return 1 si hi ha 4 en ratlla
 *
 */
int buscaColumnes(char tauler[N][N], char jugador) {
    int i,j;
    int count;
    for (j=0; j<N; j++) {
        count=0;
        for (i=0; i<N; i++) {
            if(tauler[i][j]==jugador) {
                if(++count == 4)
                    return 1;
            } else
                count = 0;
        }
    }
    return 0;
}

/**
 * Buscar 4 en ratlla del jugador (per diagonals)
 *
 * \param[in] tauler Estat del tauler després de la jugada
 * \param[in] jugador Jugador que ha posat la fitxa
 *
 * \return 0 si no hi ha 4 en ratlla
 * \return 1 si hi ha 4 en ratlla
 *
 */
int buscaDiagonals(char tauler[N][N], char jugador) {
    int i,j;
    for (i=0; i<N-3; i++) {
        for (j=0; j<N-3; j++) {
            if (tauler[i][j] == jugador &&
                    tauler[i+1][j+1] == jugador &&
                    tauler[i+2][j+2] == jugador &&
                    tauler[i+3][j+3] == jugador)
                return 1;
        }
    }
    for (i=N-1; i>3; i--) {
        for (j=0; j<N-3; j++) {
            if (tauler[i][j] == jugador &&
                    tauler[i-1][j+1] == jugador &&
                    tauler[i-2][j+2] == jugador &&
                    tauler[i-3][j+3] == jugador)
                return 1;
        }
    }
    return 0;
}


/**
 * Fer una partida de Connecta-4
 *
 * \param[in] primer Node inicial de la partida
 * \param[in] nivells Profunditat de l'arbre del MiniMax
 * \param[in] puntuacio_pc Puntuació del PC
 * \param[in] puntuacio_huma Puntuació de l'usuari
 *
 * \return HUMA Si guanya l'usuari
 * \return ORDI Si guanya el PC
 * \return BUIT Si hi ha empat
 * \return 'E'  Si hi ha error
 *
 */
char ferPartida(Node *primer, int nivells, int puntuacio_pc, int puntuacio_huma) {
    int tira_huma,tirada;
    helloHuman(puntuacio_pc, puntuacio_huma);
    printTauler(primer->tauler);
    while (!taulerPle(primer->tauler)) {
        tira_huma = tiraHuma(primer);
        if(tira_huma == 1) {
            helloHuman(puntuacio_pc, puntuacio_huma);
            printTauler(primer->tauler);
            printf("Has guanyat! No em crec que siguis humà.\n");
            return HUMA;
        } else if (tira_huma == -1) {
            fprintf(stderr,"Error: no es pot realitzar la tirada humana.\n");
            return 'E'; // 'E' de Error
        } else {
            primer->n_fills = determinaFills(primer->tauler);
            creaArbre(primer,nivells);
            tirada=ferMinimax(primer,nivells);
            //recorreArbre(primer,1);
            esborraArbre(primer,nivells);
            if(aplicaTirada(primer->tauler,tirada,ORDI)) {
                helloHuman(puntuacio_pc, puntuacio_huma);
                printTauler(primer->tauler);
                printf("El teu cervell humà no té res a fer.\n");
                return ORDI;
            }   
            helloHuman(puntuacio_pc, puntuacio_huma);
            printTauler(primer->tauler);
        }
    }
    printf("Empat??? A què jugues, humà?\n");
    return BUIT;
}
