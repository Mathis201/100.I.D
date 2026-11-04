#include "orange.h"
#include "capteurs/capteur_distance_ir.h"
#include "motors/motor_management.h"
#include "motors/servo_management.h"
#include <LibRobus.h>

static long ticksDepartGauche;
static long ticksDepartDroit;
static float angleTourne;
const int onePeriod = 1000;
static unsigned long timeTarget = 0;
static bool hasTimerStarted = false;

static float turnSpeed = 70.0;
static const float modifiedSpeed = 150.0;
static const float valeurProcheVerre = 550;
static float seuilDetection = 325.0;
static float seuilTooClose = 400.0;
static const float forwardDistanceMm = 600.0;
static const float backwardsDistanceMm = 150.0;
static float angleMagique;

static long ticksApprocheGauche;
static long ticksApprocheDroit;
static float distanceParcourueMm = 0.0;
const float mmParTick = (2.0 * PI * 38.1) / 3200.0;
static bool tooClose = false;

static int compteur1 = 0;

enum orangeStates {
    TURN_IN_PLACE,
    STOP_TURN,
    ADJUST_TO_CUP,
    MOVE_BACKWARDS,
    LOWER_ARM,
    MOVE_TO_CUP,
    STOP_BEFORE_CUP,
    CLOSE_CLAW,
    RAISE_ARM,
    RETURN_TO_LINE,
    RESTORE_ORIENTATION,
    TEST_FINISHED
};

orangeStates orangeState;

void initOrangeAction() {
    orangeState = TURN_IN_PLACE;

    hasTimerStarted = false;
    timeTarget = 0;
    tooClose = false;
    compteur1 = 0;
    angleMagique = 8;
    openClaw();

    ticksDepartGauche = ENCODER_Read(LEFT);
    ticksDepartDroit = ENCODER_Read(RIGHT);
    requestTurnDegrees(360.0, turnSpeed);
}

void updateOrangeAction() {
    switch (orangeState) {
    case TURN_IN_PLACE:
        updateMotorProfiles();
        if (detecterObjetIR(1, seuilDetection)) {
            compteur1 += 1;
        } else {
            compteur1 = 0;
        }
        if (compteur1 >= 6) // Lecture > 120
        {
            //Serial.print("DETECTION DERRIERE:");
            //Serial.println(ROBUS_ReadIR(1));

            if (detecterObjetIR(1, seuilTooClose)) {
                angleMagique = 7;
                tooClose = true;
            }
            decelerateToStop(); // Regarder si on met une décélération
            orangeState = STOP_TURN;
        } else if (areProfilesDone()) {
            requestTurnDegrees(360.0, turnSpeed);
            seuilDetection -= 25.0;
        }
        break;

    case STOP_TURN:
        updateMotorProfiles();
        if (areProfilesDone() && tooClose) {
            requestStraightDistance(backwardsDistanceMm);
            orangeState = MOVE_BACKWARDS;
        } else if (areProfilesDone()) {
            orangeState = LOWER_ARM;
        }
        break;

    case MOVE_BACKWARDS:
        updateMotorProfiles();
        if (areProfilesDone()) {
            stopMotors();
            orangeState = LOWER_ARM;
        }
        break;

    case LOWER_ARM:
        lowerArm();
        if (!hasTimerStarted) {
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()) {
            hasTimerStarted = false;
            requestTurnDegrees(-angleMagique);
            orangeState = ADJUST_TO_CUP;
        }
        break;

    case ADJUST_TO_CUP:
        updateMotorProfiles();
        if (areProfilesDone()) {
            angleTourne = getAngleSince(ticksDepartGauche, ticksDepartDroit);
            ticksApprocheGauche = ENCODER_Read(LEFT);
            ticksApprocheDroit = ENCODER_Read(RIGHT);
            requestStraightDistance(-forwardDistanceMm, 0.0, modifiedSpeed);
            orangeState = MOVE_TO_CUP;
        }
        break;

    case MOVE_TO_CUP:
        updateMotorProfiles();
        if (detecterObjetIR(1, valeurProcheVerre)) {
            decelerateToStop();
            orangeState = STOP_BEFORE_CUP;
        } else if (areProfilesDone()) {
            stopMotors();
            long deltaGauche = ENCODER_Read(LEFT) - ticksApprocheGauche;
            long deltaDroite = ENCODER_Read(RIGHT) - ticksApprocheDroit;

            distanceParcourueMm = (deltaGauche + deltaDroite) / 2.0 * mmParTick;

            hasTimerStarted = false;
            orangeState = RAISE_ARM;
        }
        break;

    case STOP_BEFORE_CUP:
        updateMotorProfiles();
        if (areProfilesDone()) {
            stopMotors();

            long deltaGauche = ENCODER_Read(LEFT) - ticksApprocheGauche;
            long deltaDroite = ENCODER_Read(RIGHT) - ticksApprocheDroit;
            distanceParcourueMm = (deltaGauche + deltaDroite) / 2.0 * mmParTick;

            orangeState = CLOSE_CLAW;
        }
        break;

    case CLOSE_CLAW:
        closeClaw();
        if (!hasTimerStarted) {
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()) {
            orangeState = RAISE_ARM;
            hasTimerStarted = false;
        }
        break;

    case RAISE_ARM:
        raiseArm();
        if (!hasTimerStarted) {
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()) {
            hasTimerStarted = false;
            float distanceRetourMm = -distanceParcourueMm;
            if (tooClose) {
                distanceRetourMm -= backwardsDistanceMm;
            }
            requestStraightDistance(distanceRetourMm);
            orangeState = RETURN_TO_LINE; // A MODIFEIR
        }
        break;

    case RETURN_TO_LINE:
        updateMotorProfiles();
        if (areProfilesDone()) {
            stopMotors();
            requestTurnDegrees(-(angleTourne));
            orangeState = RESTORE_ORIENTATION;
        }
        break;

    case RESTORE_ORIENTATION:
        updateMotorProfiles();
        if (areProfilesDone()) {
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

/*
if (ROBUS_IsBumper(LEFT))
    {
        currentColorAction = ActionColors:: ORANGE;
        initOrangeAction();
        currentRobotState = DO_COLOR_ACTION;
    }
*/