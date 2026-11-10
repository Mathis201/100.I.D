// placer_bateau_aleatoirement.cpp
#include <cstdlib>
#include "placer_bateau_aleatoirement.h"

void calculer_offsets(char orientation, int &offsetX, int &offsetY)
{
    offsetX = 0;
    offsetY = 0;

    if (orientation == 'D')
    {
        offsetX = 1;
    }
    else if (orientation == 'G')
    {
        offsetX = -1;
    }
    else if (orientation == 'H')
    {
        offsetY = -1;
    }
    else if (orientation == 'B')
    {
        offsetY = 1;
    }
}

bool validation(int grille[taille_grille][taille_grille], int x, int y,
                int longueur_bateau, char orientation)
{
    int offsetX, offsetY;
    calculer_offsets(orientation, offsetX, offsetY);

    for (int i = 0; i < longueur_bateau; i++)
    {

        if (x < 0 || x >= taille_grille || y < 0 || y >= taille_grille)
        {
            return false;
        }
        if (grille[y][x] != 0)
        {
            return false;
        }

        x += offsetX;
        y += offsetY;
    }
    return true;
}

void rajouter_bateau(int grille[taille_grille][taille_grille], int x, int y,
                     int numero_bateau, int longueur_bateau, char orientation)
{
    int offsetX, offsetY;
    calculer_offsets(orientation, offsetX, offsetY);

    for (int i = 0; i < longueur_bateau; i++)
    {
        grille[y][x] = numero_bateau;
        x += offsetX;
        y += offsetY;
    }
}

void placer_bateau(int grille[taille_grille][taille_grille],
                   int liste_bateau[], int nb_bateau)
{
    const char orientations[4] = {'H', 'B', 'G', 'D'};

    // remet la grille à 0 à chaque appel de la fonction
    for (int i = 0; i < taille_grille; i++)
    {
        for (int j = 0; j < taille_grille; j++)
        {
            grille[i][j] = 0;
        }
    }

    for (int b = 0; b < nb_bateau; b++)
    {
        int longueur = liste_bateau[b];
        bool valide = false;

        while (!valide)
        {
            char orientation = orientations[rand() % 4];
            int x = rand() % taille_grille;
            int y = rand() % taille_grille;

            if (validation(grille, x, y, longueur, orientation))
            {
                rajouter_bateau(grille, x, y, b + 1, longueur, orientation);
                valide = true;
            }
        }
    }
}
