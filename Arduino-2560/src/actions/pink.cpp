#include "pink.h"
#include <LibRobus.h>
#include "capteurs/capteur_distance_ir.h"
#include "motors/motor_management.h"

static float distanceMaxQuille = 50.0;
static long ticksDepartGauche;
static long ticksDepartDroite;
static float distance_cm;
static float angleTourne;


enum PinkStates {
    TURN_IN_PLACE,
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
    requestTurnDegrees(360.0);
}

void updatePinkAction() {
    switch (pinkstate) {
    case TURN_IN_PLACE:
        //TODO: Tourner sur place, pas trop rapidement
        distance_cm = detecter_distance(0); //capteur IR 0
        if (distance_cm > 0.0 && distance_cm <= distanceMaxQuille)
        {
            stopMotors(); //Regarder si on met une décélération
            pinkstate = MOVE_TO_QUILLE;
            requestStraightDistance(-(distance_cm + 2.0)*10.0);
            angleTourne = getAngleSince(ticksDepartGauche, ticksDepartDroite);
        }
        else if (areProfilesDone())
        {
            requestTurnDegrees(360.0);  // À revoir, parce que le robot va refaire tous les tours après
            distanceMaxQuille += 10.0;  // On pourrait mettre initPinkAction et la distanceMaxQuille
        }
        break;
    case MOVE_TO_QUILLE: // sinon ça pourrait être case QUILLE_DETECTED, puis l'approche de la quille va suivre
        // TODO: avancer vers la quille
        //if(distance_parcourue == distance_cm)
        if (areProfilesDone())
        {
            stopMotors(); //décélération?
            pinkstate = RETURN_TO_LINE;
            requestStraightDistance((distance_cm + 2.0) * 10.0);
        }
        break;
    case RETURN_TO_LINE:
        if (areProfilesDone())
        {
            stopMotors();
            requestTurnDegrees(-angleTourne);
            pinkstate = RESTORE_ORIENTATION;
        }
        break;
    case RESTORE_ORIENTATION:
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
