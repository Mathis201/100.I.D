#include "pink.h"
#include <LibRobus.h>
#include "capteurs/capteur_distance_ir.h"
#include "motors/motor_management.h"

static float distanceMaxQuille = 50.0;
static long ticksDepartGauche;
static long ticksDepartDroite;
static float distance_cm;
static float angleTourne;
static float turnSpeed = 50.0;

enum PinkStates {
    TURN_IN_PLACE,
    STOP_TURN,
    MOVE_TO_QUILLE,
    RETURN_TO_LINE,
    RESTORE_ORIENTATION,
    TEST_FINISHED
};

PinkStates pinkstate;


void initPinkAction() {
    pinkstate = TURN_IN_PLACE;

    ticksDepartGauche = ENCODER_Read(LEFT);
    ticksDepartDroite = ENCODER_Read(RIGHT);
    requestTurnDegrees(360.0, turnSpeed);
}

void updatePinkAction() {
    switch (pinkstate) {
    case TURN_IN_PLACE:
        distance_cm = detecter_distance(0); //capteur IR 0
        updateMotorProfiles();
        if (distance_cm > 0.0 && distance_cm <= distanceMaxQuille)
        {
            decelerateToStop();
            pinkstate = STOP_TURN;
        }
        else if (areProfilesDone())
        {
            requestTurnDegrees(360.0, turnSpeed);
            distanceMaxQuille += 10.0;  // On pourrait mettre initPinkAction et la distanceMaxQuille
        }
        break;
    case STOP_TURN:
        updateMotorProfiles();
        if (areProfilesDone())
        {   
            angleTourne = getAngleSince(ticksDepartGauche, ticksDepartDroite);
            requestStraightDistance((distance_cm + 10.0) * 10.0);
            pinkstate = MOVE_TO_QUILLE;
        }
        break;
    case MOVE_TO_QUILLE: // sinon ça pourrait être case QUILLE_DETECTED, puis l'approche de la quille va suivre
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors(); //décélération?
            pinkstate = RETURN_TO_LINE;
            requestStraightDistance(-(distance_cm + 10.0) * 10.0);
        }
        break;
    case RETURN_TO_LINE:
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors();
            requestTurnDegrees(-angleTourne);
            pinkstate = RESTORE_ORIENTATION;
        }
        break;
    case RESTORE_ORIENTATION:
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors();
            pinkstate = TEST_FINISHED;
        }
        break;
    case TEST_FINISHED:
        break;
    }
}

bool isPinkActionFinished() {
    return pinkstate == TEST_FINISHED;
}
