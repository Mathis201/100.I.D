#include "pink.h"
#include <LibRobus.h>
#include "capteurs/capteur_distance_ir.h"
#include "motors/motor_management.h"

static long ticksDepartGauche;
static long ticksDepartDroite;
static float angleTourne;
static float turnSpeed = 60;
static const float modifiedSpeed = 150.0;
static const float valeurProcheQuille = 550;
static float seuilDetection1 = 300.0;
static const float forwardDistanceMm = 600.0;
static const float angleMagique = 3;
static int compteurTours = 0;

static long ticksApprocheGauche;
static long ticksApprocheDroit;
static float distanceParcourueMm = 0.0;
const float mmParTick = (2.0 * PI * 38.1) / 3200.0;

static int compteur1 = 0;

enum PinkStates {
    TURN_IN_PLACE,
    STOP_TURN,
    ADJUST_TO_QUILLE,
    MOVE_TO_QUILLE,
    SPEED_TO_QUILLE,
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

    compteur1 = 0;

}

void updatePinkAction() {
    switch (pinkstate) {
    case TURN_IN_PLACE:{
        updateMotorProfiles();

        int lectureArriere = ROBUS_ReadIR(1);
        
        if (lectureArriere >= seuilDetection1)
        {
        compteur1 += 1;
        }
        else
        {
            compteur1 = 0;
        }

        bool arriereDetecte = compteur1 >= 5;

        if (arriereDetecte)
        {
            decelerateToStop();
            pinkstate = STOP_TURN;
        }
        else if (areProfilesDone())
        {
            if (compteurTours >= 1)
            {
                pinkstate = TEST_FINISHED;
            }
            requestTurnDegrees(360.0, turnSpeed);
            seuilDetection1 -= 25.0;
            compteurTours += 1;
        }
    }
        break;
    case STOP_TURN:
        updateMotorProfiles();
        if (areProfilesDone())
        {   
            angleTourne = getAngleSince(ticksDepartGauche, ticksDepartDroite);
            requestTurnDegrees(-angleMagique);
            pinkstate = ADJUST_TO_QUILLE;
        }
        break;
    case ADJUST_TO_QUILLE:
        updateMotorProfiles();
        if (areProfilesDone())
        {
        ticksApprocheGauche = ENCODER_Read(LEFT);
        ticksApprocheDroit = ENCODER_Read(RIGHT);
        requestStraightDistance(-forwardDistanceMm, 0.0, modifiedSpeed);
        pinkstate = MOVE_TO_QUILLE;
        }
        break;
    case MOVE_TO_QUILLE: // sinon ça pourrait être case QUILLE_DETECTED, puis l'approche de la quille va suivre
        updateMotorProfiles();
        if (detecterObjetIR(1, valeurProcheQuille))
        {
            requestStraightDistance(-100);
            pinkstate = SPEED_TO_QUILLE;
        }
        else if (areProfilesDone())
        {
            stopMotors(); //décélération?
            pinkstate = SPEED_TO_QUILLE;
        }
        break;
    
    case SPEED_TO_QUILLE:
        updateMotorProfiles();

        if (areProfilesDone())
        {
            stopMotors();

            long deltaGauche = ENCODER_Read(LEFT) - ticksApprocheGauche;
            long deltaDroit = ENCODER_Read(RIGHT) - ticksApprocheDroit;
            distanceParcourueMm = (deltaGauche + deltaDroit) / 2.0 * mmParTick;
            requestStraightDistance(-distanceParcourueMm);
            pinkstate = RETURN_TO_LINE;
        }
        break;
    case RETURN_TO_LINE:
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors();
            requestTurnDegrees(-(angleTourne - angleMagique));
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
