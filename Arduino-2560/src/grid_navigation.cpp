#include "grid_navigation.h"

constexpr static int gridXSize = 10;
constexpr static int gridYSize = 3;

// hardcoded walls, encoded as coordinates
constexpr static int xTape[][2] = {{}};
constexpr static int yTape[][2] = {{1, 1}, {1, 2}, {3, 1}, {3, 2}, {5, 1}, {5, 2}, {7, 1}, {7, 2}, {9, 1}, {9, 2}};

// walls that can be encountered when moving through x positions
// first index is the square row, second index is the line column
// holds WallType values to represent walls
static int xWalls[gridYSize][gridXSize + 1]{};

// walls that can be encountered when moving through y positions
// first index is the square column, second index is the line row
// holds WallType values to represent walls
static int yWalls[gridXSize][gridYSize + 1]{};

void initWalls() {
    for (int row = 0; row < gridYSize; row++) {
        for (int column = 0; column < gridXSize + 1; column++) {
            int initValue = WallType::UNKNOWN;
            // if first or last column, outline of grid, place wall
            if (column == 0 || column == gridXSize) {
                initValue = WallType::WALL;
            }

            xWalls[row][column] = initValue;
        }
    }

    for (auto xTapeCoords : xTape) {
        // place the vertical tape
        xWalls[xTapeCoords[0]][xTapeCoords[1]] = WallType::WALL;
    }

    for (int column = 0; column < gridXSize; column++) {
        for (int row = 0; row < gridYSize + 1; row++) {
            int initValue = WallType::UNKNOWN;
            // if at start or beginning of row, outline of grid, place wall
            if (row == 0 || row == gridYSize) {
                initValue = WallType::WALL;
            }

            yWalls[column][row] = initValue;
        }
    }

    for (auto yTapeCoords : yTape) {
        // place the horizontal tape
        yWalls[yTapeCoords[0]][yTapeCoords[1]] = WallType::WALL;
    }
}

int *getWallsAtPos(int squareX, int squareY) {
    static int walls[4];
    // front and back walls are x walls
    // x0 has a wall behind, so x is back and y + 1 is front
    walls[WallPos::FRONT] = xWalls[squareY][squareX + 1];
    walls[WallPos::BACK] = xWalls[squareY][squareX];

    // left and right walls are y walls
    // y0 has a wall to the right, so y is right and y + 1 is left
    walls[WallPos::LEFT] = yWalls[squareX][squareY + 1];
    walls[WallPos::RIGHT] = yWalls[squareX][squareY];

    return walls;
}

void replaceWallAtPos(int squareX, int squareY, WallType newVal, WallPos wallPos) {
    switch (wallPos) {
    case WallPos::FRONT:
        xWalls[squareY][squareX + 1] = newVal;
        break;
    case WallPos::BACK:
        xWalls[squareY][squareX] = newVal;
        break;
    case WallPos::LEFT:
        yWalls[squareX][squareY + 1] = newVal;
        break;
    case WallPos::RIGHT:
        yWalls[squareX][squareY] = newVal;
        break;
    default:
        break;
    }
}