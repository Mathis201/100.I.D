#pragma once
#include "algorithme_battleship.h"
#include "game_defs.h"

void resetGameManager();

void setRobotAlgorithmStrength(AlgorithmStrength chosenStrength);

void setPlayerCreatedGrid(int newPlayerGrid[kGridSize][kGridSize]);

Coord getNextRobotShot();