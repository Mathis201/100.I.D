#include "game_manager.h"
#include "algorithme_battleship.h"
#include "game_defs.h"
#include "placer_bateau_aleatoirement.h"

static AlgorithmStrength robotAlgorithmStrength;
static EtatOrdi robotAlgorithmState;

static int playerCreatedGrid[kGridSize][kGridSize] = {};
static int playerAttacksGrid[kGridSize][kGridSize] = {};

static int robotGeneratedGrid[kGridSize][kGridSize] = {};
static int robotAttacksGrid[kGridSize][kGridSize] = {};

void zeroOutGrid(int grid[kGridSize][kGridSize]) {
    for (int i = 0; i < kGridSize; i++) {
        for (int j = 0; j < kGridSize; j++) {
            grid[i][j] = 0;
        }
    }
}

void resetGameManager() {
    robotAlgorithmStrength = AlgorithmStrength::NORMAL;
    robotAlgorithmState = EtatOrdi();

    zeroOutGrid(playerAttacksGrid);
    zeroOutGrid(robotAttacksGrid);
    zeroOutGrid(playerCreatedGrid);
    placer_bateau(robotGeneratedGrid, kBoatLengths, kNumberOfBoats);

    initialiser_ordi(robotAlgorithmState);
}

void setRobotAlgorithmStrength(AlgorithmStrength chosenStrength) {
    robotAlgorithmStrength = chosenStrength;
}

void setPlayerCreatedGrid(int newPlayerGrid[kGridSize][kGridSize]) {
    for (int i = 0; i < kGridSize; i++) {
        for (int j = 0; j < kGridSize; j++) {
            playerCreatedGrid[i][j] = newPlayerGrid[i][j];
        }
    }
}

Coord getNextRobotShot() {
    return effectuer_prochain_coup(robotAttacksGrid, playerCreatedGrid, robotAlgorithmState, robotAlgorithmStrength);
}

ShotResult registerRobotShot(Coord robotShot) {
    return evaluer_coup(robotShot, playerCreatedGrid, robotAttacksGrid);
}

ShotResult registerPlayerShot(Coord playerShot) {
    return evaluer_coup(playerShot, robotGeneratedGrid, playerAttacksGrid);
}