#include "yellow.h"
#include "capteurs/line_sensor.h"
#include "motors/motor_management.h"
#include <Arduino.h>
#include <LibRobus.h>

enum YellowStates {
    WAIT_FOR_WALL,
    TURN_1,
    FORWARD_1,
    TURN_2,
    FORWARD_2,
    TURN_3,
    FORWARD_3,
    TURN_4,
    FINISHED
};

// Liste de constante pour le programme
bool StartRotation = false; // Condition pour la rotation
bool StartForward = false;  // Condition pour avancer
const int SEUIL_IR = 270;   // Valeur numerique donner par arduino qu'on dit au robot d'Arreter devant le mur

YellowStates CurrentStates = WAIT_FOR_WALL; // etat initial

uint16_t LireDistanceMur() {
    return ROBUS_ReadIR(0);
}
void initYellowAction() {
    CurrentStates = WAIT_FOR_WALL;
    StartForward = false;
    StartRotation = false;
}

bool isYellowActionFinished() {
    return CurrentStates == FINISHED;
}

// TODO: merge la logique de sam dans les fonctions prédéfinies

void updateYellowAction() {
    switch (CurrentStates) {
    case WAIT_FOR_WALL: {
        // Lire la distance du capteur:

        uint16_t valeurIR = LireDistanceMur();
        // Serial.println("Valeur IR : ");
        // Serial.print(valeurIR);// test pour distance du mur
        // delay (10000);

        if (valeurIR >= SEUIL_IR) { // Condition pour arreter devant le mur

            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            CurrentStates = TURN_1;
        } else {
            // Continue à suivre la ligne
            Adapt_Curve();
        }
        break;
    }

    case TURN_1: { // Premier virage de 90 degrés

        if (StartRotation == false) {
            requestTurnDegrees(-90);

            StartRotation = true;
        }

        updateMotorProfiles();

        if (areProfilesDone()) // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FORWARD_1;
        }

        break;
    }

    case FORWARD_1: { // Avancement du robot
        if (StartForward == false) {
            requestStraightDistance(500);

            StartForward = true;
        }
        updateMotorProfiles();

        if (areProfilesDone()) {

            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartForward = false;
            CurrentStates = TURN_2;
        }
        break;
    }

    case TURN_2: { // Deuxième tournant pour contourner le mur
        if (StartRotation == false) {
            requestTurnDegrees(90);

            StartRotation = true;
        }
        updateMotorProfiles();

        if (areProfilesDone()) // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FORWARD_2;
        }

        break;
    }

    case FORWARD_2: {
        if (StartForward == false) {
            requestStraightDistance(600);

            StartForward = true;
        }
        updateMotorProfiles();

        if (areProfilesDone()) {

            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartForward = false;
            CurrentStates = TURN_3;
        }
        break;
    }

    case TURN_3: {
        if (StartRotation == false) {
            requestTurnDegrees(90);

            StartRotation = true;
        }
        updateMotorProfiles();

        if (areProfilesDone()) // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FORWARD_3;
        }

        break;
    }

    case FORWARD_3: {
        if (StartForward == false) {
            requestStraightDistance(800);

            StartForward = true;
        }

        updateMotorProfiles();

        if (IsLineDetected(3) || areProfilesDone()) { // Trouver la ligne apres avoir contourner le mur 
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartForward = false;
            CurrentStates = TURN_4;
        }
        break;
    }

    case TURN_4: { // Derniere rotation sur la ligne 
        if (StartRotation == false) {
            requestTurnDegrees(-90);

            StartRotation = true;
        }
        updateMotorProfiles();

        if (areProfilesDone()) // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FINISHED;
        }

        break;
    }
    default:
        break;
    }
}
