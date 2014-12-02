/**
 * \file main.c
 * \brief Programa que usa minimax.c i 4enratlla.c per poder jugar al joc
 * \author Oscar Saleta Reig
 */
#include <stdio.h>
#include "minimax.h"
#include "4enratlla.h"

/**
 * Netejar el buffer per no llegir "\n"
 *
 * \return Res
 */
void clearBuffer() {
    while ( getchar()!='\n' );
}

int dificultat; ///< Profunditat de l'arbre, variable global necessària per recorreArbre()

/**
 * Funció principal del programa
 *
 * \return 0 Si el programa acaba correctament
 */
int main (void) {
    Node *primer;
    char seguirJugant='S';
    char guanyador;
    int puntuacio_pc=0, puntuacio_huma=0;
    
    while (seguirJugant=='S') {
        printf("Introdueix el nivell de dificultat: ");
        scanf("%d",&dificultat);
        primer=inicialitzaJoc();
        guanyador=ferPartida(primer,dificultat,puntuacio_pc,puntuacio_huma);
        switch(guanyador) {
            case ORDI:
                puntuacio_pc++;
                break;
            case HUMA:
                puntuacio_huma++;
                break;
        }
        printf("\nVols seguir jugant? [S/n] ");
        clearBuffer();
        seguirJugant = getchar();
        printf("\n");
        if (seguirJugant!='n')
            seguirJugant='S';
    }

    return 0;

}
