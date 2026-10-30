#include "blue.h"
#include "LibRobus.h"
#include "motors/motor_management.h"

bool requestedStop = false;

void initBlueAction() {
    // requestStraightDistance(10000);
    // requestedStop = false;
}

void updateBlueAction() {
    // if (ROBUS_IsBumper(LEFT) && !requestedStop) {
    //     decelerateToStop();
    //     requestedStop = true;
    // }

    // updateMotorProfiles();
}

bool isBlueActionFinished() {
    // bool finished = requestedStop && areProfilesDone();
    // if (finished) {
    //     stopMotors();
    // }
    // return (finished);
    return true;
}