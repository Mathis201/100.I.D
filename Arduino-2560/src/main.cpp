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

constexpr int pin5hz = A0;
constexpr int pinBA = A1;

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

void celebrer() {
    // Refrain traditionnel de La Cucaracha : frequences en hertz.
    int notes[17] = {
        523, 523, 523, 698, 880,
        523, 523, 523, 698, 880,
        698, 698, 659, 659, 587, 587, 523};
    int durees[17] = {
        150, 150, 200, 300, 450,
        150, 150, 200, 300, 450,
        200, 200, 200, 200, 200, 200, 1300};

    // Deux repetitions de 5 secondes chacune.
    for (int repetition = 0; repetition < 2; repetition++) {
        for (int note = 0; note < 17; note++) {
            // Petite pause entre les notes pour bien distinguer les bips.
            AX_BuzzerON(notes[note], durees[note] - 30);
            delay(durees[note]);
        }
    }
}

bool detection_sifflet() {
    int val5Hz = analogRead(pin5hz);
    int valBA = analogRead(pinBA);

    Serial.print("5 kHz: ");
    Serial.print(val5Hz);
    Serial.print("BA: ");
    Serial.print(valBA);
    Serial.print(", différence: ");
    Serial.println(val5Hz - valBA);

    return ((val5Hz - valBA) >= 70);
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

    // if (ROBUS_IsBumper(RIGHT)) {
    //     currentState = FINISHED;
    // }

    switch (currentState) {
    case WAIT_FOR_START: {
        if (ROBUS_IsBumper(REAR) || detection_sifflet()) {
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
            stopMotors();
            celebrer();
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
        stopMotors();
        currentState = WAIT_FOR_START;
        break;
    }
    }

    delay(10);
}
