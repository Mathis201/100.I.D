#ifndef ALGORITHME_BATTLESHIP_H
#define ALGORITHME_BATTLESHIP_H

#include "placer_bateau_aleatoirement.h" // fournit taille_grille

const int NB_BATEAUX = 5;

enum Resultat { RATE, TOUCHE, COULE };

// choix du mode de l'algo
enum ModeOrdi { ALEATOIRE, NORMAL, TRICHE };

struct Coord {
    int x;
    int y;
};

struct Move {
    Coord coord;
    int longueur;
};

struct EtatOrdi {
    ModeOrdi mode;
    int nb_de_coup;
    Resultat dernier_resultat;
    Coord dernier_touche;
    bool bateau_actif;
    int actif_code;
    int actif_longueur;
    Move next_boat_to_do[NB_BATEAUX];
    int nb_en_attente;
};

void initialiser_ordi(EtatOrdi &etat);
bool coord_valide(Coord coup);
bool case_inconnue(int x, int y,
                   const int grille_vue[taille_grille][taille_grille]);
Coord coup_aleatoire(const int grille_vue[taille_grille][taille_grille]);
bool choisir_orientation(Coord &coup,
                         const int grille_vue[taille_grille][taille_grille],
                         int actif_code);

// choisit le tir en fonction du mode de jeu, seulement le mode triche utilise la grille reelle
Coord effectuer_prochain_coup(
    const int grille_vue[taille_grille][taille_grille],
    const int grille_bateaux_adversaire[taille_grille][taille_grille],
    EtatOrdi &etat);

// evalue et retourne le resulat d'un tir avec la grille reelle
Resultat evaluer_coup(
    Coord coup,
    const int grille_cible[taille_grille][taille_grille],
    int grille_vue_tireur[taille_grille][taille_grille],
    int restantes_cible[NB_BATEAUX + 1]);

void mettre_a_jour_stats_ordi(
    Coord coup, Resultat resultat,
    const int grille_vue_ordi[taille_grille][taille_grille],
    EtatOrdi &etat);

#endif
