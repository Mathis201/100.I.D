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
    FINISHED
};

constexpr float transitionTime = 40;

int loopCounter = 0;

RobotStates currentState;
float targetTimerTime = 0;
bool timerStarted = false;

void beep(int count) {
    for (int i = 0; i < count; i++) {
        AX_BuzzerON();
        delay(100);
        AX_BuzzerOFF();
        delay(100);
    }
    delay(400);
}

void setup() {
    BoardInit();

    beep(1);

    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    currentState = WAIT_FOR_START;
}

void loop() {
    updateMotorInfo(millis());

    switch (currentState) {
    case WAIT_FOR_START: {
        loopCounter = 0;
        if (ROBUS_IsBumper(REAR)) {
            delay(300);
            currentState = FIND_NEXT_MOVE;
        }
        break;
    }
    case FIND_NEXT_MOVE: {
        timerStarted = false;

        if (loopCounter >= 8) {
            currentState = FINISHED;
        } else {
            if (loopCounter % 2 == 0) {
                requestStraightDistance(500);
            } else {
                requestTurnDegrees(90);
            }
            currentState = MOVE;
        }
        break;
    }
    case MOVE: {
        if (areProfilesDone() && !timerStarted) {
            targetTimerTime = millis() + transitionTime;
            timerStarted = true;
        } else {
            updateMotorProfiles();
        }

        if (timerStarted && millis() >= targetTimerTime) {
            loopCounter++;
            currentState = FIND_NEXT_MOVE;
        }
        break;
    }
    case FINISHED: {
        stopMotors();
        beep(1);
        currentState = WAIT_FOR_START;
        break;
    }
    }
}
