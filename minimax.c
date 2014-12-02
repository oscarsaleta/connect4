/*! \mainpage Treball en C per Programació Avançada
 * \author Oscar Saleta Reig
 */

/*! \file minimax.c 
 * \brief Fitxer amb funcions genèriques de MiniMax
 * \author Oscar Saleta Reig
 */
#include <stdio.h>
#include <stdlib.h>
#include "minimax.h"
#include "4enratlla.h"


extern int dificultat; ///< Nivell de dificultat desat com a variable global

/**
 * Crea un node a partir d'un tauler i una tirada
 * (modifica el tauler amb la tirada, compta els fills i els assigna memòria)
 *
 * \param[in] tauler Tauler del node pare
 * \param[in] num_tirada Columna lliure on fem la tirada (començant per 0)
 * \param[in] jugador Pot ser HUMA o ORDI, indica qui fa la jugada
 *
 * \return Node amb el tauler actualitzat amb la tirada del jugador
 *
 */
Node *creaNode (char tauler[N][N], int num_tirada, char jugador) {
    Node *p=malloc(sizeof(Node)); // punter per poder retornar-lo (si fos local es perdria)
    copiaTauler(tauler,p->tauler); // funció que copiarà el tauler que ens passen al Node creat
    aplicaTirada(p->tauler,num_tirada,jugador); // funció que aplicarà la tirada al tauler de *p
    p->n_fills = determinaFills(p->tauler); // funció que compta el nº de columnes lliures del tauler de *p
    p->fills = malloc(p->n_fills * sizeof(Node *));
    return p;
}


/**
 * Crea els fills d'un node
 *
 * \param[in] pare Node a partir del qual crearem tots els fills (possibles jugades)
 * \param[in] jugador Pot ser HUMA o ORDI, indica qui fa la jugada
 * 
 * \param[out] pare Fills creats amb les diferents jugades possibles
 *                         a partir de pare
 * \return Res
 *
 */
void crea1Nivell (Node *pare, char jugador) {
    int i;
    for (i=0; i<pare->n_fills; i++)
        pare->fills[i] = creaNode(pare->tauler, i, jugador); // Aquí decidim que num_tirada comença per 0!
}


/**
 * A partir d'un node crea els seus fills i els fills dels fills, etc., fins la profunditat desitjada
 *
 * \param[in] arrel Node a partir del qual creem l'arbre
 * \param[in] nivells Profunditat de l'arbre (a partir de 1)
 *
 * \param[out] arrel Tot l'arbre amb totes les jugades possibles
 *
 * \return Res
 *
 */
void creaArbre (Node *arrel, int nivells) {
    int i,j;
    crea1Nivell(arrel, ORDI); // creem els fills per l'arrel
    for (i=0; i<arrel->n_fills; i++) {
        crea1Nivell(arrel->fills[i],HUMA); // per cada fill de l'arrel creem els seus fills
        if (nivells>1)
            for (j=0; j<arrel->fills[i]->n_fills; j++)
                creaArbre(arrel->fills[i]->fills[j],nivells-1); // si nivells>1, creem un altre arbre de cada fulla, amb nivells-1
        else
            for (j=0; j<arrel->fills[i]->n_fills; j++)
                arrel->fills[i]->fills[j]->valor = 
                    valoraNode(arrel->fills[i]->fills[j]->tauler); // si hem arribat al fons, avaluem els nodes
    }
}


/**
 * Recorre l'arbre i imprimeix el valor de cada node
 *
 * \param[in] arrel Node inicial
 * \param[in] nivells Profunditat de l'arbre
 *
 * \return Res
 *
 */
void recorreArbre (Node *arrel, int nivells) { // Crida inicial: nivells=1, incrementarem els nivells recurrentment
    int i,j,k;
    for (k=0; k<nivells; k++)
        printf("--");
    printf("> %g\n", arrel->valor);
    for (i=0; i<arrel->n_fills; i++) {
        for (k=0; k<nivells; k++)
            printf("--");
        printf("-> %g\n",arrel->fills[i]->valor);
        if (nivells<dificultat) {
            for (j=0; j<arrel->fills[i]->n_fills; j++) {
                recorreArbre(arrel->fills[i]->fills[j], nivells+1);
            }
        }
        else {
            for (j=0; j<arrel->fills[i]->n_fills; j++) {
                for (k=0; k<nivells; k++)
                    printf("  ");
                printf("    %g\n", arrel->fills[i]->fills[j]->valor);
            }
        }
    }
       
}


/**
 * Esborrar un sol node de l'arbre
 *
 * \param[in] node Node a esborrar (no hauria de tenir fills)
 * 
 * \return Res
 *
 */
void esborraNode(Node *node) {
    free(node->fills);
    free(node);
}


/**
 * Esborrar un nivell d'un arbre (els fills)
 *
 * \param[in] arrel Node del qual hem d'esborrar els fills
 *
 * \return Res
 *
 */
void esborra1Nivell (Node *arrel) {
    int i;
    for (i=0; i<arrel->n_fills; i++)
        esborraNode(arrel->fills[i]);
}


/**
 * Esborrar un arbre sencer de profunditat arbitrària
 *
 * \param[in] arrel Node de partida
 * \param[in] nivells Profunditat de l'arbre
 *
 * \return Res
 *
 */
void esborraArbre (Node *arrel, int nivells) {
    int i,j;
    if (nivells-1 > 0) {
        for (i=0; i<arrel->n_fills; i++) {
            for (j=0; j<arrel->fills[i]->n_fills; j++)
                esborraArbre(arrel->fills[i]->fills[j],nivells-1);
        }
    }
    for (i=0; i < arrel->n_fills; i++)
        esborra1Nivell(arrel->fills[i]);
    esborra1Nivell(arrel);
}


/**
 * Comptar el nombre de jugades possibles, donat un tauler (màx. N)
 *
 * \param[in] tauler Tauler del qual es compten el nombre de jugades possibles
 *
 * \return Nombre de fills (entre 0 i N)
 *
 */
int determinaFills (char tauler[N][N]) {
    int i;
    int fills = 0;
    for (i=0; i<N; i++)
        if (tauler[0][i] == BUIT)
            fills++;
    return fills;
}


/**
 * Mirar quina columna buida correspon a la tirada introduïda
 *
 * \param[in] tauler Tauler al qual es tira
 * \param[in] num_tirada Nombre de la columna buida 
 *                       (diferent al nombre de columna, en general)
 *
 * \return Nombre de columna entre 0 i N-1, ò -1 si no n'hi ha de buides.
 *
 */
int determinaColumna (char tauler[N][N], int num_tirada) {
    int i;
    int aux_col=-1;
    for (i=0; i<N; i++) {
        if (tauler[0][i] == BUIT)
            aux_col++;
        if (aux_col == num_tirada)
            return i;
    }
    return -1;
}


/**
 * Mira a quina fila caurà la fitxa (per gravetat)
 *
 * \param[in] tauler Tauler al qual es tira
 * \param[in] columna Columna (entre 0 i N-1) a la que es tira
 *
 * \return Nombre de fila (entre 0 i N-1), ò -1 si la columna estava plena
 *
 */
int determinaFila (char tauler[N][N], int columna) {
    int i;
    for (i=N-1; i>=0; i--) {
        if (tauler[i][columna] == BUIT)
            return i;
    }
    return -1;
}


/**
 * Llegir la tirada de l'humà i aplicar-la al tauler actual
 *
 * \param[in,out] actual Estat actual de la partida
 * 
 * \return aplicaTirada(actual->tauler,num_tirada-1,HUMA)
 *
 */
int tiraHuma (Node *actual) {
    int i;
    int fills,num_tirada=N+1;
    fills = determinaFills(actual->tauler);
    printf("Tirades disponibles: ");
    // imprimim les tirades disponibles numerades a partir de 1
    for (i=0; i<fills; i++)
        printf("[%d] ",i+1);
    printf("\nIntrodueix tirada:\n");
    while(num_tirada > fills || num_tirada <= 0) 
        fscanf(stdin,"%d",&num_tirada);
    return aplicaTirada(actual->tauler,num_tirada-1,HUMA);
}


/**
 * Copiar el contingut d'un tauler a un altre
 *
 * \param[in] input Tauler d'entrada
 * \param[out] output Tauler de sortida (copiem input)
 *
 * \return Res
 *
 */
void copiaTauler(char input[N][N], char output[N][N]) {
    int i,j;
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            output[i][j] = input[i][j];
        }
    }   
}


/**
 * Imprimir el tauler en format "agradable" pel jugador (X jugador, O ordinador)
 *
 * \param[in] tauler Tauler a imprimir
 *
 * \return Res
 *
 */
void printTauler (char tauler[N][N]) {
    int i,j;
    for (i=0; i<N; i++) {
        printf("|");
        for (j=0; j<N; j++)
            printf(" %c |",tauler[i][j]);
        printf("\n");
    }
    for (i=0; i<N; i++)
        printf("====");
    printf("\n");
}


/**
 * Aplicar l'algorisme MiniMax recurrentment per trobar la tirada òptima
 *
 * \param[in] arrel Node arrel de l'arbre a analitzar
 * \param[in] nivells Nivell de profunditat de recurrència de l'arbre
 *
 * \return Tirada (nº de columna buida)
 *
 */
int ferMinimax (Node *arrel, int nivells) {
    int i,j;
    int tirada=rand()%8;
    double min=2e6,max=-2e6;
    
    if (nivells>1) {
        for (i=0; i<arrel->n_fills; i++) {
            for (j=0; j<arrel->fills[i]->n_fills; j++) {
                ferMinimax(arrel->fills[i]->fills[j], nivells-1);
            }
        }
    }
    // Assignem als fills de l'arrel el mínim de les fulles
    for (i=0; i<arrel->n_fills; i++) {
        min=2e6;
        for (j=0; j<arrel->fills[i]->n_fills; j++) {
            if (arrel->fills[i]->fills[j]->valor < min)
                min = arrel->fills[i]->fills[j]->valor;
        }
        arrel->fills[i]->valor = min;
        // Mirem si aquest valor és major que el mínim calculat
        if (min > max) {
            tirada = i;
            max = min;
        }
        arrel->valor = max;
    }
    return tirada;
}


/**
 * Mirar si el tauler està ple
 *
 * \param[in] tauler Tauler a estudiar
 *
 * \return 1 si està ple, 0 si no ho està
 *
 */
int taulerPle (char tauler[N][N]) {
    if(!determinaFills(tauler))
        return 1;
    return 0;
}


/** 
 * Donar un valor heurístic al node basat la bondat del seu tauler (suposant que juga HUMA pròximament)
 * 
 * \param[in] tauler Tauler a valorar
 *
 * \return Valoració del tauler
 *
 */
double valoraNode (char tauler[N][N]) { 
    int winLines_ordi, winLines_huma;
    if (busca4(tauler,ORDI))
        return 1e6;
    else if (busca4(tauler,HUMA))
        return -1e6;
    else {
        winLines_ordi= winningCols(tauler,ORDI)+winningRows(tauler,ORDI)+winningDiagonals(tauler,ORDI);
        winLines_huma= winningCols(tauler,HUMA)+winningRows(tauler,HUMA)+winningDiagonals(tauler,HUMA);
        return winLines_ordi-winLines_huma;
    }
}


/**
 * Mirar les "columnes guanyadores": grups de 4 caselles per columnes amb al menys una fitxa del
 * jugador i cap fitxa del contrari
 *
 * \param[in] tauler Tauler a estudiar
 * \param[in] jugador A qui afavoreixen les columnes guanyadores
 *
 * \return Nombre de columnes guanyadores
 *
 */
int winningCols (char tauler[N][N], char jugador) {
    int i,j,k;
    int isWinCol; // 1 si la columna és guanyadora, 0 si no ho és (ho sumarem al nº de columnes guanyadores)
    int winCols=0;
    for (i=N-1; i>3; i--) {
        for (j=0; j<N; j++) {
            if (tauler[i][j] == jugador) {
                isWinCol=1;
                for (k=1; k<4; k++) {
                    if (tauler[i+k][j] == altreJugador(jugador)) {
                        isWinCol=0;
                        break;
                    }
                }
                winCols+=isWinCol;
            }
        }
    }
    return winCols;
}


/**
 * Mirar les "files guanyadores": grups de 4 caselles per files amb al menys una fitxa del
 * jugador i cap fitxa del contrari
 *
 * \param[in] tauler Tauler a estudiar
 * \param[in] jugador A qui afavoreixen les files guanyadores
 *
 * \return Nombre de files guanyadores
 */
int winningRows (char tauler[N][N], char jugador) {
    int i,j,k;
    int isWinRow; // 1 si la fila és guanyadora, 0 si no ho és (ho sumarem al nº de files guanyadores)
    int winRows=0;
    // començo a mirar de baix a dalt
    for (i=N-1; i>0; i--) {
        // i d'esquerra a dreta (sense passar de la posició N-3)
        for (j=0; j<N-3; j++) {
            // si trobem una casella del jugador, comptem fila guanyadora a la dreta
            if (tauler[i][j] == jugador) {
                isWinRow=1;
                for (k=1; k<4; k++) {
                    // si alguna de les 3 caselles posteriors són de l'altre jugador, no és fila guanyadora
                    if (tauler[i][j+k] == altreJugador(jugador)) {
                        isWinRow=0;
                        break;
                    }
                }
                winRows+=isWinRow;
            }
            // també pot ser fila guanyadora si comença buida
            // o sigui, una sola fitxa aïllada pot arribar a donar 4 files guanyadores
            if (tauler[i][j] == BUIT) {
                isWinRow=0;
                for (k=1;k<4;k++) {
                    if(tauler[i][j+k] == altreJugador(jugador)) {
                        isWinRow=0;
                        break;
                    }
                    if(tauler[i][j+k] == jugador)
                        isWinRow=1;
                }
                winRows+=isWinRow;
            }
        }
    }
    return winRows;
}


/**
 * Mirar les "diagonals guanyadores": grups de 4 caselles per diagonals amb al menys una fitxa del
 * jugador i cap fitxa del contrari
 *
 * \param[in] tauler Tauler a estudiar
 * \param[in] jugador A qui afavoreixen les diagonals guanyadores
 *
 * \return Nombre de diagonals guanyadores
 *
 */
int winningDiagonals (char tauler[N][N], char jugador) {
    int i,j,k;
    int isWinDiag; // 1 si la diagonal és guanyadora, 0 si no ho és (ho sumarem al nº de diagonals guanyadores)
    int winDiags=0;
    // diagonals ascendents cap a la dreta
    for (i=N-1; i>3; i--) {
        for (j=0; j<N-3; j++) {
            if (tauler[i][j] == jugador) {
                isWinDiag=1;
                for (k=1; k<4; k++) {
                    if (tauler[i-k][j+k] == altreJugador(jugador)) {
                        isWinDiag=0;
                        break;
                    }
                }
                winDiags+=isWinDiag;
            }
        }
    }
    // diagonals descendents cap a la dreta
    for (i=N-4; i>0; i--) {
        for (j=0; j<N-3; j++) {
            if (tauler[i][j] == jugador) {
                isWinDiag=1;
                for (k=1; k<4; k++) {
                    if (tauler[i+k][j+k] == altreJugador(jugador)) {
                        isWinDiag=0;
                        break;
                    }
                }
                winDiags+=isWinDiag;
            }
        }
    }
    return winDiags;
}


/**
 * Donat un jugador, torna el seu contrincant
 *
 * \param[in] jugador És un dels jugadors
 *
 * \return El jugador contrari a l'introduit
 *
 */
char altreJugador (char jugador) {
    if (jugador == ORDI)
        return HUMA;
    return ORDI;
}
