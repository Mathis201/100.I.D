#include "orange.h"
#include "motors/servo_management.h"
#include "motors/motor_management.h"
#include "capteurs/capteur_distance_ir.h"
#include <LibRobus.h>

static float distanceMaxVerre = 50.0;
static long ticksDepartGauche;
static long ticksDepartDroite;
static float distance_cm;
static float angleTourne;
const int onePeriod = 1000;
unsigned long timeTarget = 0;
bool hasTimerStarted = false;
static float turnSpeed = 60.0;
static const float ajustementVerreCm = 11.0;
static const float modifiedSpeed = 150.0;


enum orangeStates {
    TURN_IN_PLACE,
    STOP_TURN,
    LOWER_ARM,
    MOVE_TO_CUP,
    CLOSE_CLAW,
    RAISE_ARM,
    RETURN_TO_LINE,
    RESTORE_ORIENTATION,
    TEST_FINISHED
};

orangeStates orangeState;

void initOrangeAction() {
    orangeState = TURN_IN_PLACE;

    distanceMaxVerre = 50.0;
    hasTimerStarted = false;
    timeTarget = 0;

    ticksDepartGauche = ENCODER_Read(LEFT);
    ticksDepartDroite = ENCODER_Read(RIGHT);
    requestTurnDegrees(360.0, turnSpeed);
}

void updateOrangeAction() {
    switch (orangeState){
    case TURN_IN_PLACE:
        distance_cm = detecter_distance(0); //capteur IR 0
        updateMotorProfiles();
        if (distance_cm > 0.0 && distance_cm <= distanceMaxVerre)
        {
            decelerateToStop(); //Regarder si on met une décélération
            orangeState = STOP_TURN;
        }
        else if (areProfilesDone())
        {
            requestTurnDegrees(360.0, turnSpeed);
            distanceMaxVerre += 10.0;
        }
        break;

    case STOP_TURN:
        updateMotorProfiles();
        if (areProfilesDone())
        {   
            angleTourne = getAngleSince(ticksDepartGauche, ticksDepartDroite);
            orangeState = LOWER_ARM;
        }
        break;
    
    case LOWER_ARM:
        lowerArm();
        if (!hasTimerStarted){
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()){
            hasTimerStarted = false;
            requestStraightDistance(-(distance_cm) * 10.0, 0.0, modifiedSpeed);
            orangeState = MOVE_TO_CUP;
        }
    break;

    case MOVE_TO_CUP:
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors(); //décélération?
            orangeState = CLOSE_CLAW;
        }
        break;

    case CLOSE_CLAW:
        closeClaw();
        if (!hasTimerStarted){
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()){
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
        if (timeTarget <= millis()){
            hasTimerStarted = false;
            requestStraightDistance((distance_cm) * 10.0);
            orangeState = RETURN_TO_LINE; //A MODIFEIR
        }
        break;

    case RETURN_TO_LINE:
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors();
            requestTurnDegrees(-angleTourne);
            orangeState = RESTORE_ORIENTATION;
        }
        break;

    case RESTORE_ORIENTATION:
        updateMotorProfiles();
        if (areProfilesDone())
        {
            stopMotors();
            orangeState = TEST_FINISHED;
        }
        break;

    case TEST_FINISHED:
        break;
    }
}

bool isOrangeActionFinished() {
    return orangeState == TEST_FINISHED;
}