// battleship.cpp
// Grille 8x8, bateaux de longueurs 2, 2, 3, 4, 5
// Coordonnees : x = colonne (0-7), y = ligne (0-7) : grille[y][x].
//
// Grille vue joueur :  0 = inconnu, -1 = rate,
//                      20 / 21 = bateaux de longueur 2, 30 / 40 / 50 = longueurs 3 / 4 / 5,
//                      codes negatifs = bateau coule (-20, -21, -30, -40, -50).

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "placer_bateau_aleatoirement.h"   // fournit taille_grille et placer_bateau()
using namespace std;

const int NB_BATEAUX = 5;

// deplacements pour HAUT, BAS, GAUCHE, DROITE
const int DX[4] = {0, 0, -1, 1};
const int DY[4] = {-1, 1, 0, 0};

enum Resultat { RATE, TOUCHE, COULE };

struct Coord {
    int x;
    int y;
};

// grille reelle : 0 = eau, 1 a 5 = id du bateau (lue seulement par evaluer_coup)
int grille_reelle[taille_grille][taille_grille];

// grille vue joueur : 0 = inconnu, -1 = rate, 20/21/30/40/50 = bateau touche,
// negatif = bateau coule
int grille_vue[taille_grille][taille_grille];

int longueur_bateau[NB_BATEAUX + 1] = {0, 2, 2, 3, 4, 5};
int cases_restantes[NB_BATEAUX + 1];
int code_bateau[NB_BATEAUX + 1];       // 0 = pas encore decouvert
int nb_bateaux_longueur_2_decouverts;  // pour donner 20 puis 21

int nb_de_coup;
Resultat dernier_resultat;
Coord dernier_touche;

bool bateau_actif; // true = on poursuit un bateau touche, mais pas encore coule
int actif_code; // code du bateau actif (20, 21, 30, 40, 50)
int actif_longueur;

int nb_bateaux_coules;

// vrai si la case est dans la grille et jamais attaquee
bool case_inconnue(int x, int y) {
    return x >= 0 && x < taille_grille && y >= 0 && y < taille_grille && grille_vue[y][x] == 0;
}

Coord coup_aleatoire() {
    Coord coup;
    do {
        coup.x = rand() % taille_grille;
        coup.y = rand() % taille_grille;
    } while (grille_vue[coup.y][coup.x] != 0);
    return coup;
}

// choisit la prochaine case pour le bateau actif, false si aucune n'est possible
bool choisir_orientation(Coord &coup) {
    // cherche les extremites des cases touchees
    int nb = 0;
    int min_x = taille_grille, max_x = -1, min_y = taille_grille, max_y = -1;
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
        // une seule touche, on essaie les 4 voisins
        for (int d = 0; d < 4; d++) {
            int nx = min_x + DX[d], ny = min_y + DY[d];
            if (case_inconnue(nx, ny)) {
                possibles[n].x = nx;
                possibles[n].y = ny;
                n++;
            }
        }
    } else if (min_y == max_y) {
        // bateau horizontal
        if (case_inconnue(min_x - 1, min_y)) { possibles[n].x = min_x - 1; possibles[n].y = min_y; n++; }
        if (case_inconnue(max_x + 1, min_y)) { possibles[n].x = max_x + 1; possibles[n].y = min_y; n++; }
    } else {
        // bateau vertical
        if (case_inconnue(min_x, min_y - 1)) { possibles[n].x = min_x; possibles[n].y = min_y - 1; n++; }
        if (case_inconnue(min_x, max_y + 1)) { possibles[n].x = min_x; possibles[n].y = max_y + 1; n++; }
    }

    if (n == 0) return false;
    coup = possibles[rand() % n];
    return true;
}

// choisit la prochaine case a attaquer (n'utilise pas grille_reelle)
Coord effectuer_prochain_coup() {
    if (bateau_actif) {
        Coord coup;
        if (choisir_orientation(coup)) {
            return coup;
        }
        bateau_actif = false;   // ne devrait pas arriver
    }
    return coup_aleatoire();
}

// 20 puis 21 pour les deux bateaux de longueur 2, sinon longueur * 10
int attribuer_code(int longueur) {
    if (longueur == 2) {
        int code = 20 + nb_bateaux_longueur_2_decouverts;
        nb_bateaux_longueur_2_decouverts++;
        return code;
    }
    return longueur * 10;
}

// regarde la grille reelle et retourne RATE, TOUCHE ou COULE
Resultat evaluer_coup(Coord coup) {
    nb_de_coup++;
    int id = grille_reelle[coup.y][coup.x];

    if (id == 0) {
        grille_vue[coup.y][coup.x] = -1;
        dernier_resultat = RATE;
        return RATE;
    }

    cases_restantes[id]--;

    bool nouveau = (code_bateau[id] == 0);
    if (nouveau) {
        code_bateau[id] = attribuer_code(longueur_bateau[id]);
    }
    int code = code_bateau[id];

    grille_vue[coup.y][coup.x] = code;
    dernier_touche = coup;

    if (cases_restantes[id] > 0) {
        dernier_resultat = TOUCHE;
        if (nouveau && !bateau_actif) {
            bateau_actif = true;
            actif_code = code;
            actif_longueur = longueur_bateau[id];
        }
        return TOUCHE;
    }

    nb_bateaux_coules++;
    dernier_resultat = COULE;

    if (bateau_actif && code == actif_code) {
        bateau_actif = false;
    }

    // met les cases du bateau coule en negatif
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            if (grille_vue[y][x] == code) {
                grille_vue[y][x] = -code;
            }
        }
    }
    return COULE;
}

void initialiser_partie() {
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            grille_reelle[y][x] = 0;
            grille_vue[y][x] = 0;
        }
    }
    for (int id = 1; id <= NB_BATEAUX; id++) {
        cases_restantes[id] = longueur_bateau[id];
        code_bateau[id] = 0;
    }
    nb_bateaux_longueur_2_decouverts = 0;

    nb_de_coup = 0;
    dernier_resultat = RATE;
    dernier_touche.x = 0;
    dernier_touche.y = 0;
    bateau_actif = false;
    actif_code = 0;
    actif_longueur = 0;
    nb_bateaux_coules = 0;
}

void afficher_grille(int grille[taille_grille][taille_grille]) {
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            if (grille[y][x] >= 0) cout << " ";
            if (grille[y][x] > -10 && grille[y][x] < 10) cout << " ";
            cout << grille[y][x] << " ";
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));
    initialiser_partie();

    placer_bateau(grille_reelle, &longueur_bateau[1], NB_BATEAUX);

    cout << "Grille reelle (cachee) :" << endl;
    afficher_grille(grille_reelle);
    cout << endl;

    while (nb_bateaux_coules < NB_BATEAUX) {
        Coord coup = effectuer_prochain_coup();
        Resultat r = evaluer_coup(coup);

        cout << "Coup " << nb_de_coup << " : (x=" << coup.x << ", y=" << coup.y << ") -> ";
        if (r == RATE) cout << "RATE";
        else if (r == TOUCHE) cout << "TOUCHE";
        else cout << "COULE";
        cout << endl;
    }

    cout << endl << "Partie terminee en " << nb_de_coup << " coups. Grille vue joueur :" << endl;
    afficher_grille(grille_vue);
    return 0;
}