#include "blue.h"
#include "LibRobus.h"
#include "capteurs/receveur_ir.h"
#include "constants.h"
#include "motors/motor_management.h"

enum BlueStates {
    DECELERATE,
    WAIT_FOR_REMOTE,
    FINISHED
};

BlueStates currentState;

void initBlueAction() {
    currentState = DECELERATE;
    decelerateToStop();
}

void updateBlueAction(int currentRemoteCommand) {
    Serial.println(currentState);
    switch (currentState) {
    case DECELERATE: {
        if (areProfilesDone()) {
            currentState = WAIT_FOR_REMOTE;
        }
        updateMotorProfiles();
        break;
    }
    case WAIT_FOR_REMOTE: {
        if (currentRemoteCommand == REMOTE_WAKE_COMMAND) {
            currentState = FINISHED;
        }
        break;
    }
    default:
        break;
    }
}

bool isBlueActionFinished() {
    return currentState == FINISHED;
}