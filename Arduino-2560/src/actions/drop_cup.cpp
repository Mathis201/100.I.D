#include "drop_cup.h"
#include "motors/servo_management.h"
#include <LibRobus.h>

const int onePeriod = 1000;
unsigned long timeTarget = 0;
bool hasTimerStarted = false;

enum dropCupStates {
    LOWER_ARM,
    OPEN_CLAW,
    TEST_FINISHED
};

dropCupStates dropCupState;

void initDropCupAction(){
    dropCupState = LOWER_ARM;
}
void updateDropCupAction(){
    switch (dropCupState)
    {
    case LOWER_ARM:
        lowerArm();
        if (!hasTimerStarted){
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()){
            dropCupState = OPEN_CLAW;
            hasTimerStarted = false;
        }  
        break;
    case OPEN_CLAW:
        openClaw();
        if (!hasTimerStarted){
            timeTarget = millis() + onePeriod;
            hasTimerStarted = true;
        }
        if (timeTarget <= millis()){
            dropCupState = OPEN_CLAW;
            hasTimerStarted = false;
        }  
        break;
    case TEST_FINISHED:
        break;
    }
}
bool isDropCupActionFinished(){
    return dropCupState == TEST_FINISHED;
}

