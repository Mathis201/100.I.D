#include "../Arduino-2560/src/grid_navigation.h"
#include <iostream>

// cd "c:\Users\simon\OneDrive\Uni\S1\100.I.D\Tests\" ; if ($?) { g++ simulateur_grille.cpp ../Arduino-2560/src/grid_navigation.cpp -o simulateur_grille } ; if ($?) { .\simulateur_grille }

constexpr int startPosX = 0;
constexpr int startPosY = 1;
constexpr int startOrientation = FRONT;

constexpr static int xWalls[][2] = {{0, 2}, {1, 5}, {0, 5}};
constexpr static int yWalls[][2] = {{4, 2}};

bool isSimulatedWallThere(int currentPosX, int currentPosY, int currentOrientation) {
    int searchedX = currentPosX;
    int searchedY = currentPosY;
    if (currentOrientation == FRONT || currentOrientation == REAR) {
        if (currentOrientation == FRONT) {
            searchedX += 1;
        }

        for (const int *wall : xWalls) {
            if (wall[0] == searchedY && wall[1] == searchedX) {
                return true;
            }
        }

        return false;
    }

    if (currentOrientation == LEFT) {
        searchedY += 1;
    }

    for (const int *wall : yWalls) {
        if (wall[0] == searchedX && wall[1] == searchedY) {
            return true;
        }
    }

    return false;
}

void printSimulatedGrid(int currentPosX, int currentPosY, int currentOrientation) {
    constexpr int printSizeX = gridXSize * 2 + 1;
    constexpr int printSizeY = gridYSize * 2 + 1;

    char grid[printSizeX][printSizeY]{};

    for (int x = 0; x < gridXSize; x++) {
        for (int y = 0; y < gridYSize; y++) {
            int *walls = getWallsAtPos(x, y);
            if (walls[FRONT] == 1) {
                grid[2 * (x + 1)][2 * y + 1] = '|';
            }
            if (walls[REAR] == 1) {
                grid[2 * x][2 * y + 1] = '|';
            }
            if (walls[LEFT] == 1) {
                grid[2 * x + 1][2 * (y + 1)] = '-';
            }
            if (walls[RIGHT] == 1) {
                grid[2 * x + 1][2 * y] = '-';
            }
        }
    }

    grid[2 * currentPosX + 1][2 * currentPosY + 1] = '1';

    for (int j = 0; j < printSizeY; j++) {
        for (int i = 0; i < printSizeX; i++) {
            char character = grid[i][printSizeY - (j + 1)];
            if (!(character == '-' || character == '|' || character == '1')) {
                character = ' ';
            }
            std::cout << character;
        }
        std::cout << std::endl;
    }
}

int main() {
    initGrid(startPosX, startPosY);
    int currentPosX = startPosX;
    int currentPosY = startPosY;
    int currentOrientation = startOrientation;

    bool runAuto = false;

    while (!isMazeFinished(currentPosX, currentPosY)) {
        printSimulatedGrid(currentPosX, currentPosY, currentOrientation);
        std::string input;
        while (input != "c" && !runAuto) {
            std::cin >> input;
            if (input == "e") {
                printExploredGrid();
            }
            if (input == "w") {
                printSquareInfoAboutPos(currentPosX, currentPosY, getWallsAtPos(currentPosX, currentPosY));
            }
            if (input == "q") {
                return 0;
            }
            if (input == "r") {
                runAuto = true;
            }
        }

        int *moveInfo = findNextMove(currentPosX, currentPosY, currentOrientation);

        std::cout << std::endl;
        if (moveInfo[2] != currentOrientation) {
            std::cout << "Rotated from " << currentOrientation << " to " << moveInfo[2] << std::endl;
        } else {
            std::cout << "Moved from (" << currentPosX << ", " << currentPosY << ") to (" << moveInfo[0] << ", " << moveInfo[1] << ")" << std::endl;
        }
        currentPosX = moveInfo[0];
        currentPosY = moveInfo[1];
        currentOrientation = moveInfo[2];

        updateWallAtPos(currentPosX, currentPosY, currentOrientation,
                        isSimulatedWallThere(currentPosX, currentPosY, currentOrientation), false);
    }

    printSimulatedGrid(currentPosX, currentPosY, currentOrientation);
    std::cout << "Finished" << std::endl;
    printExploredGrid();

    return 0;
}