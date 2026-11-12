#include "algorithme_battleship.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int grille_bateaux_ordi[taille_grille][taille_grille];
int grille_bateaux_humain[taille_grille][taille_grille];
int grille_vue_ordi[taille_grille][taille_grille];
int grille_vue_humain[taille_grille][taille_grille];

int longueur_bateau[NB_BATEAUX + 1] = {0, 2, 2, 3, 4, 5};
int cases_restantes_ordi[NB_BATEAUX + 1];
int cases_restantes_humain[NB_BATEAUX + 1];
int bateaux_ordi_coules = 0;
int bateaux_humain_coules = 0;
EtatOrdi etat_ordinateur;

void initialiser_partie() {
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            grille_bateaux_ordi[y][x] = 0;
            grille_bateaux_humain[y][x] = 0;
            grille_vue_ordi[y][x] = 0;
            grille_vue_humain[y][x] = 0;
        }
    }
    for (int id = 1; id <= NB_BATEAUX; id++) {
        cases_restantes_ordi[id] = longueur_bateau[id];
        cases_restantes_humain[id] = longueur_bateau[id];
    }
    bateaux_ordi_coules = 0;
    bateaux_humain_coules = 0;
    initialiser_ordinateur(etat_ordinateur);
}

void afficher_grille(const int grille[taille_grille][taille_grille]) {
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            if (grille[y][x] >= 0) cout << " ";
            if (grille[y][x] > -10 && grille[y][x] < 10) cout << " ";
            cout << grille[y][x] << " ";
        }
        cout << endl;
    }
}

const char* nom_resultat(Resultat r) {
    if (r == RATE) return "RATE";
    if (r == TOUCHE) return "TOUCHE";
    return "COULE";
}

Coord demander_coup_humain() {
    Coord coup;
    while (true) {
        cout << "Votre tir (x y, de 0 a " << taille_grille - 1 << ") : ";
        if (!(cin >> coup.x >> coup.y)) {
            if (cin.eof()) {
                cout << "\nFin de la saisie." << endl;
                exit(0);
            }
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Saisie invalide. Entrez deux nombres." << endl;
            continue;
        }
        if (!coord_valide(coup)) {
            cout << "Coordonnees hors de la grille." << endl;
        } else if (grille_vue_humain[coup.y][coup.x] != 0) {
            cout << "Case deja attaquee. Recommencez." << endl;
        } else {
            return coup;
        }
    }
}

int main() {
    srand(time(0));
    initialiser_partie();

    // Le placement utilise les ID 1 a 5 correspondant au longueurs 2,2,3,4,5.
    placer_bateau(grille_bateaux_ordi, &longueur_bateau[1], NB_BATEAUX);
    placer_bateau(grille_bateaux_humain, &longueur_bateau[1], NB_BATEAUX);

    cout << "Bateaux ordinateur (TEST) :\n";
    afficher_grille(grille_bateaux_ordi);
    cout << "Bateaux humain (TEST) :\n";
    afficher_grille(grille_bateaux_humain);

    while (bateaux_ordi_coules < NB_BATEAUX &&
           bateaux_humain_coules < NB_BATEAUX) {
        Coord coup_ordi = effectuer_prochain_coup(grille_vue_ordi, etat_ordinateur);
        if (!coord_valide(coup_ordi)) break;
        Resultat r_ordi = evaluer_coup(
            coup_ordi, grille_bateaux_humain,
            grille_vue_ordi, cases_restantes_humain);
        mettre_a_jour_stats_ordi(coup_ordi, r_ordi, grille_vue_ordi, etat_ordinateur);
        if (r_ordi == COULE) bateaux_humain_coules++;
        cout << "Ordi : (" << coup_ordi.x << ", " << coup_ordi.y
             << ") -> " << nom_resultat(r_ordi) << endl;

        if (bateaux_humain_coules == NB_BATEAUX) break;

        Coord coup_humain = demander_coup_humain();
        Resultat r_humain = evaluer_coup(
            coup_humain, grille_bateaux_ordi,
            grille_vue_humain, cases_restantes_ordi);
        if (r_humain == COULE) bateaux_ordi_coules++;
        cout << "Humain : (" << coup_humain.x << ", " << coup_humain.y
             << ") -> " << nom_resultat(r_humain) << endl;

        cout << "\nVue ordinateur (tirs de l'ordi) :\n";
        afficher_grille(grille_vue_ordi);
        cout << "Vue humain (vos tirs) :\n";
        afficher_grille(grille_vue_humain);
    }

    if (bateaux_humain_coules == NB_BATEAUX)
        cout << "L'ordinateur a gagne !" << endl;
    else if (bateaux_ordi_coules == NB_BATEAUX)
        cout << "L'humain a gagne !" << endl;
    else
        cout << "Partie interrompue (aucun tir disponible)." << endl;
    return 0;
}
