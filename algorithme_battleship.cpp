#include "algorithme_battleship.h"
#include <cstdlib>

const int DX[4] = {0, 0, -1, 1}; // sert a calculer les coordonnees d'un deplacement (haut, bas, gauche, droite)
const int DY[4] = {-1, 1, 0, 0};
const int codes_bateaux[NB_BATEAUX + 1] = {0, 20, 21, 30, 40, 50}; // sert d'ID pour les bateaux
void initialiser_ordi(EtatOrdi &etat) {
    etat.mode = NORMAL;
    etat.nb_de_coup = 0;
    etat.dernier_resultat = RATE;
    etat.dernier_touche = {0, 0};
    etat.bateau_actif = false;
    etat.actif_code = 0;
    etat.actif_longueur = 0;
    etat.nb_en_attente = 0;
}

bool coord_valide(Coord coup) {
    return coup.x >= 0 && coup.x < taille_grille &&
           coup.y >= 0 && coup.y < taille_grille;
}

bool case_inconnue(int x, int y,
                   const int grille_vue[taille_grille][taille_grille]) {
    return x >= 0 && x < taille_grille && y >= 0 && y < taille_grille &&
           grille_vue[y][x] == 0;
}

Coord coup_aleatoire(const int grille_vue[taille_grille][taille_grille]) {
    int inconnues = 0;
    for (int y = 0; y < taille_grille; y++)
        for (int x = 0; x < taille_grille; x++)
            if (grille_vue[y][x] == 0) inconnues++;

    if (inconnues == 0) return {-1, -1};

    int choix = rand() % inconnues;
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            if (grille_vue[y][x] == 0) {
                if (choix == 0) return {x, y};
                choix--;
            }
        }
    }
    return {-1, -1};
}

bool choisir_orientation(Coord &coup,
                         const int grille_vue[taille_grille][taille_grille],
                         int actif_code) {
    int nb = 0;
    int min_x = taille_grille, max_x = -1;
    int min_y = taille_grille, max_y = -1;

    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            if (grille_vue[y][x] == actif_code) {
                nb++;
                if (x < min_x) min_x = x;
                if (x > max_x) max_x = x;
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;
            }
        }
    }

    Coord possibles[4];
    int n = 0;
    if (nb == 1) {
        for (int d = 0; d < 4; d++) {
            int nx = min_x + DX[d], ny = min_y + DY[d];
            if (case_inconnue(nx, ny, grille_vue))
                possibles[n++] = {nx, ny};
        }
    } else if (nb >= 2 && min_y == max_y) {
        if (case_inconnue(min_x - 1, min_y, grille_vue))
            possibles[n++] = {min_x - 1, min_y};
        if (case_inconnue(max_x + 1, min_y, grille_vue))
            possibles[n++] = {max_x + 1, min_y};
    } else if (nb >= 2) {
        if (case_inconnue(min_x, min_y - 1, grille_vue))
            possibles[n++] = {min_x, min_y - 1};
        if (case_inconnue(min_x, max_y + 1, grille_vue))
            possibles[n++] = {min_x, max_y + 1};
    }

    if (n == 0) return false;
    coup = possibles[rand() % n];
    return true;
}

Coord coup_normal(
    const int grille_vue[taille_grille][taille_grille], EtatOrdi &etat) {
    if (etat.bateau_actif) {
        Coord coup;
        if (choisir_orientation(coup, grille_vue, etat.actif_code))
            return coup;
        etat.bateau_actif = false;
    }

    // reprend le plus ancien bateau en attente
    while (!etat.bateau_actif && etat.nb_en_attente > 0) {
        Move prochain = etat.next_boat_to_do[0];
        for (int i = 0; i < etat.nb_en_attente - 1; i++)
            etat.next_boat_to_do[i] = etat.next_boat_to_do[i + 1];
        etat.nb_en_attente--;

        int code = grille_vue[prochain.coord.y][prochain.coord.x];
        if (code <= 0) continue; // verif si bateau deja coule
        etat.bateau_actif = true;
        etat.actif_code = code;
        etat.actif_longueur = prochain.longueur;

        Coord coup;
        if (choisir_orientation(coup, grille_vue, etat.actif_code))
            return coup;
        etat.bateau_actif = false;
    }
    return coup_aleatoire(grille_vue);
}

Coord effectuer_prochain_coup(
    const int grille_vue[taille_grille][taille_grille],
    const int grille_bateaux_adversaire[taille_grille][taille_grille], // pour le mode triche seulement
    EtatOrdi &etat) {
    (void)grille_bateaux_adversaire; // temporaire en attendant implementation du mode triche
    switch (etat.mode) {
        case NORMAL:
            return coup_normal(grille_vue, etat);

        case ALEATOIRE:
            return coup_aleatoire(grille_vue);

        case TRICHE:
            // A faire dans une fonction coup_triche. Va utiliser grille_bateaux_adversaire. Retourne une coord invalide pour l'instant 
            return {-1, -1};
    }
    return {-1, -1};
}

Resultat evaluer_coup(
    Coord coup,
    const int grille_cible[taille_grille][taille_grille],
    int grille_vue_tireur[taille_grille][taille_grille],
    int restantes_cible[NB_BATEAUX + 1]) {
    int id = grille_cible[coup.y][coup.x];
    if (id == 0) {
        grille_vue_tireur[coup.y][coup.x] = -1;
        return RATE;
    }

    restantes_cible[id]--;
    int code = codes_bateaux[id];
    grille_vue_tireur[coup.y][coup.x] = code;
    if (restantes_cible[id] > 0) return TOUCHE;

    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            if (grille_vue_tireur[y][x] == code)
                grille_vue_tireur[y][x] = -code;
        }
    }
    return COULE;
}

void mettre_a_jour_stats_ordi(
    Coord coup, Resultat resultat,
    const int grille_vue_ordi[taille_grille][taille_grille],
    EtatOrdi &etat) {
    etat.nb_de_coup++;
    etat.dernier_resultat = resultat;
    if (resultat == RATE) return;

    etat.dernier_touche = coup;
    int code = grille_vue_ordi[coup.y][coup.x];
    if (code < 0) code = -code;

    if (resultat == COULE) {
        if (etat.bateau_actif && code == etat.actif_code)
            etat.bateau_actif = false;

        // enlever un bateau coule de la liste
        for (int i = 0; i < etat.nb_en_attente; ) {
            Coord position = etat.next_boat_to_do[i].coord;
            int code_attente = grille_vue_ordi[position.y][position.x];
            if (code_attente < 0) code_attente = -code_attente;
            if (code_attente == code) {
                for (int j = i; j < etat.nb_en_attente - 1; j++)
                    etat.next_boat_to_do[j] = etat.next_boat_to_do[j + 1];
                etat.nb_en_attente--;
            } else {
                i++;
            }
        }
        return;
    }

    if (!etat.bateau_actif) {
        etat.bateau_actif = true;
        etat.actif_code = code;
        etat.actif_longueur = (code == 20 || code == 21) ? 2 : code / 10;
    } else if (code != etat.actif_code) {
        // Ne jamais ajouter deux fois le meme
        bool deja_en_attente = false;
        for (int i = 0; i < etat.nb_en_attente; i++) {
            Coord position = etat.next_boat_to_do[i].coord;
            if (grille_vue_ordi[position.y][position.x] == code) {
                deja_en_attente = true;
                break;
            }
        }
        if (!deja_en_attente && etat.nb_en_attente < NB_BATEAUX) {
            etat.next_boat_to_do[etat.nb_en_attente] = {coup, (code == 20 || code == 21) ? 2 : code / 10};
            etat.nb_en_attente++;
        }
    }
}