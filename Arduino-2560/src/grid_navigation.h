#pragma once

#ifndef FRONT
#define FRONT 2
#define LEFT 0
#define RIGHT 1
#define REAR 3
#endif

constexpr static int gridXSize = 10;
constexpr static int gridYSize = 3;

// returns whether the current x position is the end of the grid
bool isAtEndOfGrid(int currentPosX);
// sets up the walls at the start of a maze run
void initWalls(int startX, int startY);
// change a wall at a specific square and orientation
void updateWallAtPos(int squareX, int squareY, int currentOrientation, bool wallDetected, bool fromDeadEnd);
// finds the next move to make (desired position and orientation) based on current maze state.
// returns an array of size 3: [targetPosX, targetPosY, targetOrientation]
int *findNextMove(int currentPosX, int currentPosY, int currentOrientation);

int *getWallsAtPos(int squareX, int squareY);

void printExploredGrid();