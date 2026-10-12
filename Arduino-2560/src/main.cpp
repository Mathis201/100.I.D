/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/
#include <LibRobus.h>

#include "grid_navigation.h"
#include "motor_management.h"

enum RobotStates {
    WAIT_FOR_START,
    FIND_NEXT_MOVE,
    MOVE,
    UPDATE_WALLS,
    FINISHED
};

constexpr int pinRed = 48;
constexpr int pinGreen = 49;

constexpr float transitionTime = 200;

// info about start of maze
constexpr int startPosX = 0;
constexpr int startPosY = 1;
constexpr int startOrientation = FRONT;

// info about current position in the maze
int currentPosX = 0;
int currentPosY = 0;
int currentOrientation = 0;

// info about wanted position in the maze
int targetPosX = 0;
int targetPosY = 0;
int targetOrientation = 0;

RobotStates currentState;

float targetTransitionTime = 0;
bool transitionTimerStarted = false;

void beep(int count) {
    for (int i = 0; i < count; i++) {
        AX_BuzzerON();
        delay(100);
        AX_BuzzerOFF();
        delay(100);
    }
}

void setup() {
    BoardInit();
    // set proximity detector pin modes
    pinMode(pinGreen, INPUT);
    pinMode(pinRed, INPUT);

    beep(1);

    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    currentState = WAIT_FOR_START;
}

// called in a very fast loop, nothing blocking should be put here
void loop() {
    // read motor encoders every loop
    updateMotorInfo(millis());

    if (ROBUS_IsBumper(RIGHT)) {
        currentState = FINISHED;
    }

    switch (currentState) {
    case WAIT_FOR_START: {
        if (ROBUS_IsBumper(REAR)) {
            delay(300);
            // reset position in grid
            currentPosX = startPosX;
            currentPosY = startPosY;
            currentOrientation = startOrientation;

            // reset walls
            initGrid(startPosX, startPosY);

            // verify if placed in front of wall at start
            currentState = UPDATE_WALLS;
        }
        break;
    }
    case FIND_NEXT_MOVE: {
        if (isMazeFinished(currentPosX, currentPosY)) {
            currentState = FINISHED;
        } else {
            transitionTimerStarted = false;
            int *moveInfo = findNextMove(currentPosX, currentPosY, currentOrientation);
            targetPosX = moveInfo[0];
            targetPosY = moveInfo[1];
            targetOrientation = moveInfo[2];

            if (targetOrientation != currentOrientation) {
                requestOrientation(currentOrientation, targetOrientation);
            } else if (targetPosX != currentPosX) {
                requestXPosition(currentPosX, targetPosX, currentOrientation);
            } else if (targetPosY != currentPosY) {
                requestYPosition(currentPosY, targetPosY, currentOrientation);
            }

            // requestTurnDegrees(90);

            currentState = MOVE;
        }

        break;
    }
    case MOVE: {
        if (areProfilesDone() && !transitionTimerStarted) {
            targetTransitionTime = millis() + transitionTime;
            transitionTimerStarted = true;
        } else {
            updateMotorProfiles();
        }

        if (transitionTimerStarted && millis() >= targetTransitionTime) {
            currentPosX = targetPosX;
            currentPosY = targetPosY;
            currentOrientation = targetOrientation;
            currentState = UPDATE_WALLS;
            // currentState = FINISHED;
        }
        break;
    }
    case UPDATE_WALLS: {
        bool wallDetected = !digitalRead(pinRed) || !digitalRead(pinGreen);
        updateWallAtPos(currentPosX, currentPosY, currentOrientation, wallDetected, false);

        currentState = FIND_NEXT_MOVE;
        break;
    }
    case FINISHED: {
        printExploredGrid();
        stopMotors();
        beep(1);
        currentState = WAIT_FOR_START;
        break;
    }
    }
}
