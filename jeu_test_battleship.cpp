#include "algorithme_battleship.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int grille_bateaux_humain[taille_grille][taille_grille] = {};
int grille_vue_ordi[taille_grille][taille_grille] = {};
int longueur_bateau[NB_BATEAUX] = {2, 2, 3, 4, 5};
int cases_restantes_humain[NB_BATEAUX + 1] = {0, 2, 2, 3, 4, 5};

void afficher_grille(const int grille[taille_grille][taille_grille]) {
    for (int y = 0; y < taille_grille; y++) {
        for (int x = 0; x < taille_grille; x++) {
            cout << grille[y][x] << "\t";
        }
        cout << '\n';
    }
}

int main() {
    srand(time(0));
    EtatOrdi etat_ordinateur;
    initialiser_ordi(etat_ordinateur);

    // Placement aleatoire des cinq bateaux sur la grille cible
    placer_bateau(grille_bateaux_humain, longueur_bateau, NB_BATEAUX);

    int bateaux_coules = 0;
    while (bateaux_coules < NB_BATEAUX) {
        Coord coup = effectuer_prochain_coup(grille_vue_ordi, etat_ordinateur);
        if (!coord_valide(coup) || grille_vue_ordi[coup.y][coup.x] != 0) {
            cout << "Partie interrompue : aucun tir valide disponible.\n";
            break;
        }

        Resultat resultat = evaluer_coup(
            coup, grille_bateaux_humain, grille_vue_ordi, cases_restantes_humain);
        mettre_a_jour_stats_ordi(coup, resultat, grille_vue_ordi, etat_ordinateur);
        if (resultat == COULE) bateaux_coules++;

        cout << "Coup " << etat_ordinateur.nb_de_coup << " : ("
             << coup.x << ", " << coup.y << ") -> "
             << (resultat == RATE ? "RATE" : resultat == TOUCHE ? "TOUCHE" : "COULE")
             << '\n';
    }

    cout << "\nGrille des bateaux humains :\n";
    afficher_grille(grille_bateaux_humain);
    cout << "\nGrille vue de l'ordinateur :\n";
    afficher_grille(grille_vue_ordi);
    cout << "\nBateaux coules : " << bateaux_coules << "/" << NB_BATEAUX
         << " | Nombre de coups : " << etat_ordinateur.nb_de_coup << '\n';
    return 0;
}
