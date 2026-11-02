#include "green.h"
#include "Arduino.h"
#include "capteurs/line_sensor.h"
#include "motors/motor_management.h"

enum GreenStates {
    AVANCER,
    Decel,
    Reculer,
    AVANCERSANSLIGNE,
    RETROUVERLIGNE,
    Tourner_1,
    Avancer_1,
    Tourner_2,
    Avancer_2,
    Avancer_3,
    Lignetrouver,
    TEST_FINISHED
};

GreenStates CurrentsStates;
bool wasLineDetected;

void initGreenAction() {
    CurrentsStates = Decel;
    wasLineDetected = false;
    decelerateToStop();
}

void updateGreenAction() {
    switch (CurrentsStates) {

    case Decel: {
        updateMotorProfiles();
        if (areProfilesDone()) {
            requestStraightDistance(-200);
            CurrentsStates = Reculer;
        }

        break;
    }

    case Reculer: {
        updateMotorProfiles();
        if (areProfilesDone()) {
            CurrentsStates = AVANCER;
        }

        break;
    }

    case AVANCER: {
        Adapt_Curve();
        if (IsLineDetected(3)) {
            requestStraightDistance(950, 0, 300);
            CurrentsStates = AVANCERSANSLIGNE;
        }

        break;
    }

    case AVANCERSANSLIGNE: {
        updateMotorProfiles();
        if (areProfilesDone()) {
            if (IsLineDetected(1)) {
                CurrentsStates = TEST_FINISHED;

            } else {
                requestTurnDegrees(90, 250);
                CurrentsStates = Tourner_1;
            }
        }

        break;
    }

    case Tourner_1: {
        updateMotorProfiles();

        if (areProfilesDone()) {
            requestStraightDistance(300, 0, 250);
            CurrentsStates = Avancer_1;
        }

        break;
    }
    case Avancer_1: {
        updateMotorProfiles();

        if (IsLineDetected(1) && wasLineDetected == false) { // Trouver la ligne apres avoir contourner le mur
            requestStraightDistance(82, 0, 250);
            wasLineDetected = true;
        }

        if (areProfilesDone()) {
            if (wasLineDetected) {
                requestTurnDegrees(-90, 250);
                CurrentsStates = Lignetrouver;
            } else {
                requestTurnDegrees(-180);
                CurrentsStates = Tourner_2;
            }
        }

        break;
    }

    case Tourner_2: {
        updateMotorProfiles();

        if (areProfilesDone()) {
            requestStraightDistance(600, 0, 250);
            CurrentsStates = Avancer_2;
        }

        break;
    }
    case Avancer_2: {
        updateMotorProfiles();

        if (IsLineDetected(1) && wasLineDetected == false) { // Trouver la ligne apres avoir contourner le mur
            requestStraightDistance(82, 0, 250);
            wasLineDetected = true;
        }

        if (areProfilesDone()) {
            if (wasLineDetected) {
                requestTurnDegrees(90, 250);
                CurrentsStates = Lignetrouver;
            } else {
                CurrentsStates = Lignetrouver;
            }
        }

        break;
    }

    case Lignetrouver: {
        updateMotorProfiles();

        if (IsLineDetected(1)) {
            CurrentsStates = TEST_FINISHED;
        }

        if (areProfilesDone()) {
            CurrentsStates = TEST_FINISHED;
        }

        break;
    }

    default:
        break;
    }
}

bool isGreenActionFinished() {
    if (CurrentsStates == TEST_FINISHED) {
        stopMotors();
        return true;

    } else {
        return false;
    }
}