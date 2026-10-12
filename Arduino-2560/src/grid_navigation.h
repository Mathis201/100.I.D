#pragma once

#ifndef FRONT
#define FRONT 2
#define LEFT 0
#define RIGHT 1
#define REAR 3
#endif

constexpr static int gridXSize = 10;
constexpr static int gridYSize = 3;

// sets up the walls at the start of a maze run
void initGrid(int startX, int startY);
// change a wall at a specific square and orientation
void updateWallAtPos(int squareX, int squareY, int currentOrientation, bool wallDetected, bool fromDeadEnd);
// finds the next move to make (desired position and orientation) based on current maze state.
// returns an array of size 3: [targetPosX, targetPosY, targetOrientation]
int *findNextMove(int currentPosX, int currentPosY, int currentOrientation);

int *getWallsAtPos(int squareX, int squareY);

bool isMazeFinished(int currentPosX, int currentPosY);

void printExploredGrid();
void printSquareInfoAboutPos(int squareX, int squareY, int *info);