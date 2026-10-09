#include "grid_navigation.h"

#define SIMULATION 1

#if !SIMULATION
#include <LibRobus.h>
#endif

// hardcoded walls, encoded as coordinates
constexpr static int xTape[][2] = {{}};
constexpr static int yTape[][2] = {{1, 1}, {1, 2}, {3, 1}, {3, 2}, {5, 1}, {5, 2}, {7, 1}, {7, 2}, {9, 1}, {9, 2}};

enum WallType {
    WALL_UNKNOWN = -1,
    WALL_CLEAR = 0,
    WALL_BLOCKED = 1,
};

// walls that can be encountered when moving through x positions
// first index is the square row, second index is the line column
// holds WallType values to represent walls
static int xWalls[gridYSize][gridXSize + 1]{};

// walls that can be encountered when moving through y positions
// first index is the square column, second index is the line row
// holds WallType values to represent walls
static int yWalls[gridXSize][gridYSize + 1]{};

// all squares of grid, 1 represents explored and 0 unexplored
static int gridExplored[gridXSize][gridYSize]{};

// get the neighbor square to a square based on a desired orientation
// if not in the grid, return current square
// returns [neighborX, neighborY]
static int *getNeighborSquare(int squareX, int squareY, int orientation) {
    static int neighborSquare[2];

    neighborSquare[0] = squareX;
    neighborSquare[1] = squareY;
    switch (orientation) {
    case FRONT:
        if (squareX != (gridXSize - 1)) {
            neighborSquare[0] = squareX + 1;
        }
        break;
    case REAR:
        if (squareX != 0) {
            neighborSquare[0] = squareX - 1;
        }
        break;
    case LEFT:
        if (squareY != (gridYSize - 1)) {
            neighborSquare[1] = squareY + 1;
        }
        break;
    case RIGHT:
        if (squareY != 0) {
            neighborSquare[1] = squareY - 1;
        }
        break;
    default:
        break;
    }

    return neighborSquare;
}

// from an input position, going in an orientation, find the next stop point based on current maze state
// also marks every square in the path as explored
// returns an array [nextStopX, nextStopY, movementOrientation]
static int *getNextStop(int currentPosX, int currentPosY, int movementOrientation, bool wasInDeadEnd) {
    // TODO: right now only moves one space
    int *nextSquare = getNeighborSquare(currentPosX, currentPosY, movementOrientation);
    gridExplored[nextSquare[0]][nextSquare[1]] = 1;

    static int nextStop[3];
    nextStop[0] = nextSquare[0];
    nextStop[1] = nextSquare[1];
    nextStop[2] = movementOrientation;
    return nextStop;
}

// get an array of the explored state of neighbors of a square, using orientation as index
// if neighbor doesn't exist (on edge of grid), value is -1 for that neighbor
static int *getNeighborsExploredState(int mainSquareX, int mainSquareY) {
    static int neighbors[4];
    if (mainSquareX == (gridXSize - 1)) {
        neighbors[FRONT] = -1;
    } else {
        neighbors[FRONT] = gridExplored[mainSquareX + 1][mainSquareY];
    }

    if (mainSquareX == 0) {
        neighbors[REAR] = -1;
    } else {
        neighbors[REAR] = gridExplored[mainSquareX - 1][mainSquareY];
    }

    if (mainSquareY == (gridYSize - 1)) {
        neighbors[LEFT] = -1;
    } else {
        neighbors[LEFT] = gridExplored[mainSquareX][mainSquareY + 1];
    }

    if (mainSquareY == 0) {
        neighbors[RIGHT] = -1;
    } else {
        neighbors[RIGHT] = gridExplored[mainSquareX][mainSquareY - 1];
    }

    return neighbors;
}

bool isAtEndOfGrid(int currentPosX) {
    return currentPosX == (gridXSize - 1);
}

// reinitialize the grid walls based on hardcoded obstacles
// reinitialize explored grid to 0s, except for start pos provided
void initWalls(int startX, int startY) {
    for (int row = 0; row < gridYSize; row++) {
        for (int column = 0; column < gridXSize + 1; column++) {
            int initValue = WallType::WALL_UNKNOWN;
            // if first or last column, outline of grid, place wall
            if (column == 0 || column == gridXSize) {
                initValue = WallType::WALL_BLOCKED;
            }

            xWalls[row][column] = initValue;
        }
    }

    for (auto xTapeCoords : xTape) {
        // place the vertical tape
        xWalls[xTapeCoords[0]][xTapeCoords[1]] = WallType::WALL_BLOCKED;
    }

    for (int column = 0; column < gridXSize; column++) {
        for (int row = 0; row < gridYSize + 1; row++) {
            int initValue = WallType::WALL_UNKNOWN;
            // if at start or beginning of row, outline of grid, place wall
            if (row == 0 || row == gridYSize) {
                initValue = WallType::WALL_BLOCKED;
            }

            yWalls[column][row] = initValue;
        }
    }

    for (auto yTapeCoords : yTape) {
        // place the horizontal tape
        yWalls[yTapeCoords[0]][yTapeCoords[1]] = WallType::WALL_BLOCKED;
    }

    // reinitialize explored grid
    for (int row = 0; row < gridXSize; row++) {
        for (int column = 0; column < gridYSize; column++) {
            gridExplored[row][column] = 0;
        }
    }

    // set the start pos as explored
    gridExplored[startX][startY] = 1;
}

int *getWallsAtPos(int squareX, int squareY) {
    static int walls[4];
    // front and back walls are x walls
    // x0 has a wall behind, so x is rear and x + 1 is front
    walls[FRONT] = xWalls[squareY][squareX + 1];
    walls[REAR] = xWalls[squareY][squareX];

    // left and right walls are y walls
    // y0 has a wall to the right, so y is right and y + 1 is left
    walls[LEFT] = yWalls[squareX][squareY + 1];
    walls[RIGHT] = yWalls[squareX][squareY];

    return walls;
}

void updateWallAtPos(int squareX, int squareY, int currentOrientation, bool wallDetected, bool fromDeadEnd) {
    int *walls = getWallsAtPos(squareX, squareY);

    // if wall already in memory, skip (avoid replacing tape when no wall detected)
    // only if not deadend, if from dead end want to overwrite
    if (walls[currentOrientation] != WallType::WALL_UNKNOWN && !fromDeadEnd) {
        return;
    }

    switch (currentOrientation) {
    case FRONT:
        xWalls[squareY][squareX + 1] = wallDetected;
        break;
    case REAR:
        xWalls[squareY][squareX] = wallDetected;
        break;
    case LEFT:
        yWalls[squareX][squareY + 1] = wallDetected;
        break;
    case RIGHT:
        yWalls[squareX][squareY] = wallDetected;
        break;
    default:
        break;
    }
}

void printSquareInfoAboutPos(int squareX, int squareY, int *info) {
#if !SIMULATION
    Serial.print("(");
    Serial.print(squareX);
    Serial.print(", ");
    Serial.print(squareY);
    Serial.println("):");

    Serial.print(" ");
    Serial.println(info[FRONT]);
    Serial.print(info[LEFT]);
    Serial.print(" ");
    Serial.println(info[RIGHT]);
    Serial.print(" ");
    Serial.println(info[REAR]);
    Serial.println("");
#endif
}

int *findNextMove(int currentPosX, int currentPosY, int currentOrientation) {
    int *currentWalls = getWallsAtPos(currentPosX, currentPosY);
    int *neighborsExploredState = getNeighborsExploredState(currentPosX, currentPosY);

    int numWalls = 0;
    int lastFreeDirection = -1;
    for (int i = 0; i < 4; i++) {
        if (currentWalls[i] == 1) {
            numWalls += 1;
        } else {
            lastFreeDirection = i;
        }
    }

    // nowhere to go
    if (numWalls == 4) {
        static int nextMove[3];
        nextMove[0] = currentPosX;
        nextMove[1] = currentPosY;
        nextMove[2] = currentOrientation;
        return nextMove;
    }

    // dead end, need to back up through only freePath
    if (numWalls == 3) {
        // turn to free path if not currently facing that way
        if (currentOrientation != lastFreeDirection) {
            static int nextMove[3];
            nextMove[0] = currentPosX;
            nextMove[1] = currentPosY;
            nextMove[2] = lastFreeDirection;
            return nextMove;
        }

        // otherwise move to the last free direction

        // add wall where we're reversing to avoid going back there
        updateWallAtPos(currentPosX, currentPosY, lastFreeDirection, true, true);

        // remove current square from explored (went back)
        gridExplored[currentPosX][currentPosY] = 0;

        // go back from current spot
        return getNextStop(currentPosX, currentPosY, lastFreeDirection, true);
    }

    // if not deadend or impossible, find which way to go next

    // direction priorities
    int orientationPriorityOrder[4]{FRONT, LEFT, RIGHT, REAR};
    // if not facing front, rearrange priorities
    // current orientation should be 2nd most favored (front is always favored)
    if (currentOrientation != FRONT) {
        // find current orientation's priority
        int currentOrientationIndex = 3;
        for (int i = 1; i < 4; i++) {
            if (orientationPriorityOrder[i] == currentOrientation) {
                currentOrientationIndex = i;
                break;
            }
        }

        // place current orientation as 2nd priority (index 1)
        // loop from current index down to index 1 and shift elements to the right
        for (int i = currentOrientationIndex; i > 1; i--) {
            orientationPriorityOrder[i] = orientationPriorityOrder[i - 1];
        }
        // place the current orientation in the space created by the shifts
        orientationPriorityOrder[1] = currentOrientation;
    }

    // loop through priorities to find the next move to make
    for (int i = 0; i < 4; i++) {
        int prioritizedOrientation = orientationPriorityOrder[i];

        // if neighbor in that orientation not explored and not blocked off by wall
        if (neighborsExploredState[prioritizedOrientation] == 0 &&
            currentWalls[prioritizedOrientation] != 1) {
            // if facing that way and not blocked off by walls, go that way
            if (currentOrientation == prioritizedOrientation &&
                currentWalls[prioritizedOrientation] == 0) {
                return getNextStop(currentPosX, currentPosY, prioritizedOrientation, false);
            }

            // if unknown, turn to see it
            if (currentWalls[prioritizedOrientation] == -1) {
                static int nextMove[3];
                nextMove[0] = currentPosX;
                nextMove[1] = currentPosY;
                nextMove[2] = prioritizedOrientation;
                return nextMove;
            }
        }
    }

    static int nextMove[3];
    nextMove[0] = currentPosX;
    nextMove[1] = currentPosY;
    nextMove[2] = currentOrientation;
    return nextMove;
}

void printExploredGrid() {
#if !SIMULATION
    for (int i = 0; i < gridXSize; i++) {
        for (int j = 0; j < gridYSize; j++) {
            Serial.print(gridExplored[i][j]);
        }
        Serial.println();
    }
#endif
}
