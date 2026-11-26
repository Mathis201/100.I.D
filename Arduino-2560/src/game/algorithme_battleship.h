#ifndef ALGORITHME_BATTLESHIP_H
#define ALGORITHME_BATTLESHIP_H

#include "game_defs.h" // fournit kGridSize

const int NB_BATEAUX = 5;

struct Move {
    Coord coord;
};

struct EtatOrdi {
    int nb_de_coup;
    bool bateau_actif;
    int actif_code;
    Move next_boat_to_do[NB_BATEAUX];
    int nb_en_attente;
};

void initialiser_ordi(EtatOrdi &etat);
bool coord_valide(Coord coup);
bool case_inconnue(int x, int y,
                   const int grille_vue[kGridSize][kGridSize]);
Coord coup_aleatoire(const int grille_vue[kGridSize][kGridSize]);
bool choisir_orientation(Coord &coup,
                         const int grille_vue[kGridSize][kGridSize],
                         int actif_code);

// choisit le tir en fonction du mode de jeu, seulement le mode triche utilise la grille reelle
Coord effectuer_prochain_coup(
    const int grille_vue[kGridSize][kGridSize],
    const int grille_bateaux_adversaire[kGridSize][kGridSize],
    EtatOrdi &etat,
    AlgorithmStrength modeAttaque);

// evalue et retourne le resulat d'un tir avec la grille reelle
ShotResult evaluer_coup(
    Coord coup,
    const int grille_cible[kGridSize][kGridSize],
    int grille_vue_tireur[kGridSize][kGridSize]);

void mettre_a_jour_stats_ordi(
    Coord coup, ShotResult resultat,
    const int grille_vue_ordi[kGridSize][kGridSize],
    EtatOrdi &etat);

#endif
