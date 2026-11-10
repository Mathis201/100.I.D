// placer_bateau_aleatoirement.h
// Module qui place des bateaux au hasard dans une grille.
//
// Grille : 0 = eau, 1, 2, 3... = numero du bateau (dans l'ordre de liste_bateau).

#ifndef PLACER_BATEAU_ALEATOIREMENT_H
#define PLACER_BATEAU_ALEATOIREMENT_H

const int taille_grille = 8;

// Remet la grille a 0 puis place chaque bateau de liste_bateau au hasard.
// liste_bateau contient les longueurs (ex. {2, 2, 3, 4, 5}), nb_bateau est leur nombre.
void placer_bateau(int grille[taille_grille][taille_grille],
                   int liste_bateau[], int nb_bateau);

#endif
