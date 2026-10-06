/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

#include "motor_management.h"
#include <LibRobus.h>

enum RobotStates {
    WAIT_FOR_START,
    FIND_NEXT_MOVE,
    MOVE,
    FINISHED
};

RobotStates currentState;

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
    currentState = WAIT_FOR_START;

    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    updateMotorInfo(0);
}

void loop() {
    switch (currentState) {
    case WAIT_FOR_START: {
        if (ROBUS_IsBumper(REAR)) {
            delay(300);
            currentState = FIND_NEXT_MOVE;
        }
        break;
    }
    case FIND_NEXT_MOVE: {
        break;
    }
    case MOVE: {
        break;
    }
    case FINISHED: {
        stopMotors();
        beep(3);
        currentState = WAIT_FOR_START;
        break;
    }
    }
}
