#pragma once

enum WallPos {
    FRONT,
    LEFT,
    RIGHT,
    BACK
};

enum WallType {
    UNKNOWN = -1,
    NO_WALL = 0,
    WALL = 1,
};

void initWalls();
int *getWallsAtPos(int squareX, int squareY);
void replaceWallAtPos(int squareX, int squareY, WallType newVal, WallPos wallPos);