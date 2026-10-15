#include "grid_navigation.h"

#define SIMULATION 0
#define DEADENDS_POSSIBLE 0

#if !SIMULATION
#include <LibRobus.h>
#else
#include <iostream>
#endif

// hardcoded walls, encoded as coordinates
constexpr static int xTape[][2] = {{1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 7}, {1, 8}, {1, 9}};
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

// all squares of grid, 0 represents unexplored and a number is the position of the square in the path
static int gridExplored[gridXSize][gridYSize]{};
static int currentExploredCount;

static int startPosX;
static int startPosY;

static bool reachedEnd;

enum EndSequence {
    NOT_AT_END,
    LEAVE_GRID,
    TURN_TO_MIDDLE,
    MOVE_TO_MIDDLE,
    TURN_TO_START,
    MOVE_TO_START
};

EndSequence currentEndState;

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

static void addSquareToExplored(int squareX, int squareY) {
    currentExploredCount++;
    gridExplored[squareX][squareY] = currentExploredCount;
}

static void removeSquareFromExplored(int squareX, int squareY) {
    currentExploredCount--;
    gridExplored[squareX][squareY] = 0;
}

// from an input position, going in an orientation, find the next stop point based on current maze state
// also handles explored grid status
// returns an array [nextStopX, nextStopY, movementOrientation]
static int *handleNextStopForwards(int currentPosX, int currentPosY, int movementOrientation, bool wasInDeadEnd) {
    static int currentStop[3];
    currentStop[0] = currentPosX;
    currentStop[1] = currentPosY;
    currentStop[2] = movementOrientation;

    bool canKeepGoing = true;
    while (canKeepGoing) {
        if (wasInDeadEnd && !(currentStop[0] == startPosX && currentStop[1] == startPosY)) {
            // add wall where we came from to avoid going back there
            updateWallAtPos(currentStop[0], currentStop[1], movementOrientation, true, true);

            // remove current square from explored (went back)
            removeSquareFromExplored(currentStop[0], currentStop[1]);
        }

        int *nextSquare = getNeighborSquare(currentStop[0], currentStop[1], movementOrientation);

        // if next wasn't marked as explored, mark it
        if (gridExplored[nextSquare[0]][nextSquare[1]] == 0) {
            addSquareToExplored(nextSquare[0], nextSquare[1]);
        }

        int *nextWalls = getWallsAtPos(nextSquare[0], nextSquare[1]);
        int *neighborsExploredState = getNeighborsExploredState(nextSquare[0], nextSquare[1]);
        int numWalls = 0;
        for (int i = 0; i < 4; i++) {
            if (nextWalls[i] == 1) {
                numWalls += 1;
            }
        }

        wasInDeadEnd = (numWalls == 3);

        canKeepGoing = false;
        if (numWalls >= 2) {
            if (nextWalls[movementOrientation] == 0 ||
                (nextWalls[movementOrientation] == -1 && !DEADENDS_POSSIBLE)) {
                if (neighborsExploredState[movementOrientation] == 0 || wasInDeadEnd) {
                    canKeepGoing = true;
                }
            }
        }

        currentStop[0] = nextSquare[0];
        currentStop[1] = nextSquare[1];
    }
    return currentStop;
}

static bool isAtEndOfGrid(int currentPosX) {
    return currentPosX == (gridXSize - 1);
}

static int *getNextMoveBackwards(int currentX, int currentY, int currentOrientation) {
    static int nextMove[3];
    nextMove[0] = currentX;
    nextMove[1] = currentY;
    nextMove[2] = currentOrientation;

    if (currentEndState == LEAVE_GRID) {
        nextMove[0] = currentX + 1;
        currentEndState = TURN_TO_MIDDLE;
    } else if (currentEndState == TURN_TO_MIDDLE) {
        int desiredOrientation;
        if (currentY == 0) {
            desiredOrientation = LEFT;
        } else if (currentY == (gridYSize - 1)) {
            desiredOrientation = RIGHT;
        }
        nextMove[2] = desiredOrientation;
        currentEndState = MOVE_TO_MIDDLE;
    } else if (currentEndState == MOVE_TO_MIDDLE) {
        nextMove[1] = 1;
        currentEndState = TURN_TO_START;
    } else if (currentEndState == TURN_TO_START) {
        nextMove[2] = REAR;
        currentEndState = MOVE_TO_START;
    } else if (currentEndState == MOVE_TO_START) {
        nextMove[0] = 0;
    }
    return nextMove;
}

// reinitialize the grid walls based on hardcoded obstacles
// reinitialize explored grid to 0s, except for start pos provided
void initGrid(int startX, int startY) {
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

    currentExploredCount = 0;
    addSquareToExplored(startX, startY);

    startPosX = startX;
    startPosY = startY;
    reachedEnd = false;

    currentEndState = NOT_AT_END;
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

int *findNextMove(int currentPosX, int currentPosY, int currentOrientation) {
    if (isAtEndOfGrid(currentPosX)) {
        if (currentEndState == NOT_AT_END) {
            currentEndState = LEAVE_GRID;
        }
        reachedEnd = true;
    }

    if (isMazeFinished(currentPosX, currentPosY)) {
        static int nextMove[3];
        nextMove[0] = currentPosX;
        nextMove[1] = currentPosY;
        nextMove[2] = currentOrientation;
        return nextMove;
    }

    if (reachedEnd) {
        return getNextMoveBackwards(currentPosX, currentPosY, currentOrientation);
    }

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
        return handleNextStopForwards(currentPosX, currentPosY, lastFreeDirection, true);
    }

    // if not deadend or impossible, find which way to go next
    int orientationPriorityOrder[4]{FRONT, LEFT, RIGHT, REAR};
    // if facing right, rearrange priorities to priotize that way
    // front is always most favored and rear least favored
    if (currentOrientation == RIGHT) {
        orientationPriorityOrder[1] = RIGHT;
        orientationPriorityOrder[2] = LEFT;
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
                return handleNextStopForwards(currentPosX, currentPosY, prioritizedOrientation, false);
            }

            // otherwise, unknown or clear but not aligned
            // turn to see it
            static int nextMove[3];
            nextMove[0] = currentPosX;
            nextMove[1] = currentPosY;
            nextMove[2] = prioritizedOrientation;
            return nextMove;
        }
    }

    // all neighbors explored
    int currentPathPos = gridExplored[currentPosX][currentPosY];
    if (currentPathPos != 1) {
        int nextPosDirection = -1;
        for (int i = 0; i < 4; i++) {
            int *neighbor = getNeighborSquare(currentPosX, currentPosY, i);
            // look for neighbor with explored counter one less than current (next in path)
            if (gridExplored[neighbor[0]][neighbor[1]] == (currentPathPos - 1)) {
                nextPosDirection = i;
                break;
            }
        }

        if (nextPosDirection != -1) {
            // turn to free path if not currently facing that way
            if (currentOrientation != nextPosDirection) {
                static int nextMove[3];
                nextMove[0] = currentPosX;
                nextMove[1] = currentPosY;
                nextMove[2] = nextPosDirection;
                return nextMove;
            }

            // otherwise move to the last free direction
            return handleNextStopForwards(currentPosX, currentPosY, lastFreeDirection, true);
        }
    }

    static int nextMove[3];
    nextMove[0] = currentPosX;
    nextMove[1] = currentPosY;
    nextMove[2] = currentOrientation;
    return nextMove;
}

bool isMazeFinished(int currentPosX, int currentPosY) {
    return reachedEnd && (currentPosX == startPosX) && (currentPosY == startPosY);
}

void printExploredGrid() {
    for (int j = 0; j < gridYSize; j++) {
        for (int i = 0; i < gridXSize; i++) {
            int exploredStatus = gridExplored[i][gridYSize - (j + 1)];
#if !SIMULATION
            Serial.print(exploredStatus);
            Serial.print(" ");
#else
            std::cout << exploredStatus << " ";
#endif
        }
#if !SIMULATION
        Serial.println();
#else
        std::cout << std::endl;
#endif
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
#else
    std::cout << "(" << squareX << ", " << squareY << "):" << std::endl;
    std::cout << " " << info[FRONT] << std::endl;
    std::cout << info[LEFT] << " " << info[RIGHT] << std::endl;
    std::cout << " " << info[REAR] << std::endl
              << std::endl;
#endif
}