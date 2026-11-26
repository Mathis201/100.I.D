#include "placer_bateau_aleatoirement.h"
#include "game_defs.h"
#include "stdlib.h"

void calculer_offsets(char orientation, int &offsetX, int &offsetY) {
    offsetX = 0;
    offsetY = 0;

    if (orientation == 'D') {
        offsetX = 1;
    } else if (orientation == 'G') {
        offsetX = -1;
    } else if (orientation == 'H') {
        offsetY = -1;
    } else if (orientation == 'B') {
        offsetY = 1;
    }
}

bool validation(int grille[kGridSize][kGridSize], int x, int y,
                int longueur_bateau, char orientation) {
    int offsetX, offsetY;
    calculer_offsets(orientation, offsetX, offsetY);

    for (int i = 0; i < longueur_bateau; i++) {

        if (x < 0 || x >= kGridSize || y < 0 || y >= kGridSize) {
            return false;
        }
        if (grille[y][x] != 0) {
            return false;
        }

        x += offsetX;
        y += offsetY;
    }
    return true;
}

void rajouter_bateau(int grille[kGridSize][kGridSize], int x, int y,
                     int numero_bateau, int longueur_bateau, char orientation) {
    int offsetX, offsetY;
    calculer_offsets(orientation, offsetX, offsetY);

    for (int i = 0; i < longueur_bateau; i++) {
        grille[y][x] = numero_bateau;
        x += offsetX;
        y += offsetY;
    }
}

void placer_bateau(int grille[kGridSize][kGridSize],
                   const int liste_bateau[], int nb_bateau) {
    const char orientations[4] = {'H', 'B', 'G', 'D'};

    // remet la grille à 0 à chaque appel de la fonction
    for (int i = 0; i < kGridSize; i++) {
        for (int j = 0; j < kGridSize; j++) {
            grille[i][j] = 0;
        }
    }

    for (int b = 0; b < nb_bateau; b++) {
        int longueur = liste_bateau[b];
        bool valide = false;

        while (!valide) {
            char orientation = orientations[rand() % 4];
            int x = rand() % kGridSize;
            int y = rand() % kGridSize;

            if (validation(grille, x, y, longueur, orientation)) {
                rajouter_bateau(grille, x, y, b + 1, longueur, orientation);
                valide = true;
            }
        }
    }
}
