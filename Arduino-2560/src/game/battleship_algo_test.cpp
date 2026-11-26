#include <Arduino.h>
#include "game_manager.h"
#include "placer_bateau_aleatoirement.h"
#include "game_defs.h"

const AlgorithmStrength TEST_MODE = AlgorithmStrength::NORMAL; // algo to test

static bool testerAlgorithme(int grille[kGridSize][kGridSize]) {
    bool dejaTire[kGridSize][kGridSize] = {};
    int nbCoups = 0;
    int nbTouches = 0;
    int nbRates = 0;
    int nbCoules = 0;
    bool erreur = false;

    resetGameManager();
    setPlayerCreatedGrid(grille);
    setRobotAlgorithmStrength(TEST_MODE);

    while (nbCoules < kNumberOfBoats && nbCoups < kGridSize * kGridSize) {
        Coord coup = getNextRobotShot();

        if (coup.x < 0 || coup.x >= kGridSize ||
            coup.y < 0 || coup.y >= kGridSize) {
            Serial.println("ERREUR : coordonnees invalides");
            erreur = true;
            break;
        }

        if (dejaTire[coup.y][coup.x]) {
            Serial.println("ERREUR : tir sur une case deja utilisee");
            erreur = true;
            break;
        }

        dejaTire[coup.y][coup.x] = true;
        ShotResult resultat = registerRobotShot(coup);
        nbCoups++;

        Serial.print("Coup ");
        Serial.print(nbCoups);
        Serial.print(" : (");
        Serial.print(coup.x);
        Serial.print(", ");
        Serial.print(coup.y);
        Serial.print(") -> ");

        if (resultat == ShotResult::MISS) {
            nbRates++;
            Serial.println("MISS");
        } else if (resultat == ShotResult::HIT) {
            nbTouches++;
            Serial.println("HIT");
        } else if (resultat == ShotResult::SINK) {
            nbTouches++;
            nbCoules++;
            Serial.println("SINK");
        } else {
            Serial.println("ERREUR : resultat inconnu");
            erreur = true;
            break;
        }
    }

    int nombreCasesBateaux = 0;
    for (int i = 0; i < kNumberOfBoats; i++) {
        nombreCasesBateaux += kBoatLengths[i];
    }

    bool reussi = !erreur && nbCoules == kNumberOfBoats && nbTouches == nombreCasesBateaux && nbCoups == nbTouches + nbRates;

    if (TEST_MODE == AlgorithmStrength::PROF) {
        reussi = reussi && nbCoups == nombreCasesBateaux;
    }

    Serial.println("----- BILAN -----");
    Serial.print("Coups : ");
    Serial.println(nbCoups);
    Serial.print("Touches : ");
    Serial.println(nbTouches);
    Serial.print("Rates : ");
    Serial.println(nbRates);
    Serial.print("Bateaux coules : ");
    Serial.println(nbCoules);

    if (reussi) {
        Serial.println("TEST REUSSI");
    } else {
        Serial.println("TEST ECHOUE");
    }

    return reussi;
}

void runBattleshipTests() {
    int grille[kGridSize][kGridSize] = {};
    placer_bateau(grille, kBoatLengths, kNumberOfBoats);

    Serial.println("===== TEST ALGO BATTLESHIP =====");
    Serial.print("Mode : ");

    switch (TEST_MODE) {
    case AlgorithmStrength::RANDOM:
        Serial.println("RANDOM");
        break;
    case AlgorithmStrength::NORMAL:
        Serial.println("NORMAL");
        break;
    case AlgorithmStrength::PROF:
        Serial.println("PROF");
        break;
    default:
        Serial.println("UNKNOWN");
        break;
    }

    testerAlgorithme(grille);
}
