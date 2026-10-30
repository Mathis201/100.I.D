#include "orange.h"
#include "motors/servo_management.h"
#include "motors/motor_management.h"
#include "capteurs/capteur_distance_ir.h"
#include <LibRobus.h>

static float distanceMaxQuille = 50.0;
static long ticksDepartGauche;
static long ticksDepartDroite;
static float distance_cm;
static float angleTourne;
const int onePeriod = 1000;
unsigned long timeTarget = 0;
bool hasTimerStarted = false;

enum orangeStates {
    TURN_IN_PLACE,
    MOVE_TO_CUP,
    CLOSE_CLAW,
    RAISE_ARM,
    RETURN_TO_LINE,
    RESTORE_ORIENTATION,
    TEST_FINISHED
};

orangeStates orangeState;

void initOrangeAction() {
    orangeState = CLOSE_CLAW;

    ticksDepartGauche = ENCODER_Read(LEFT);
    ticksDepartDroite = ENCODER_Read(RIGHT);
    requestTurnDegrees(360.0);
}

void updateOrangeAction() {
    switch (orangeState){
    case TURN_IN_PLACE:
        //TODO: Tourner sur place, pas trop rapidement
        distance_cm = detecter_distance(0); //capteur IR 0
        if (distance_cm > 0.0 && distance_cm <= distanceMaxQuille)
        {
            stopMotors(); //Regarder si on met une décélération
            orangeState = MOVE_TO_CUP;
            requestStraightDistance(-(distance_cm + 2.0)*10.0);
            angleTourne = getAngleSince(ticksDepartGauche, ticksDepartDroite);
        }
        else if (areProfilesDone())
        {
            requestTurnDegrees(360.0);  // À revoir, parce que le robot va refaire tous les tours après
            distanceMaxQuille += 10.0;  // On pourrait mettre initPinkAction et la distanceMaxQuille
        }
        break;
    
    case MOVE_TO_CUP:
        // TODO: avancer vers la quille
        //if(distance_parcourue == distance_cm)
        if (areProfilesDone())
        {
            stopMotors(); //décélération?
            orangeState = CLOSE_CLAW;
            requestStraightDistance((distance_cm) * 10.0);
        }
        break;

    case CLOSE_CLAW:
        closeClaw();
        if (!hasTimerStarted){
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget >= millis()){
            orangeState = RAISE_ARM;
            hasTimerStarted = false;
        }  
        break;

    case RAISE_ARM:
        raiseArm();
        if (!hasTimerStarted){
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget >= millis()){
            orangeState = RETURN_TO_LINE;
        }
        break;

    case RETURN_TO_LINE:
        break;

    case RESTORE_ORIENTATION:
        break;

    case TEST_FINISHED:
        break;
    }
}

bool isOrangeActionFinished() {
    return orangeState == TEST_FINISHED;
}