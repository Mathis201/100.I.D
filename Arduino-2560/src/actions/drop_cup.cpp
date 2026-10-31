#include "drop_cup.h"
#include "motors/servo_management.h"
#include <LibRobus.h>

const int OnePeriod = 1000;
unsigned long TimeTarget = 0;
bool HasTimerStarted = false;

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
        if (!HasTimerStarted){
            TimeTarget = millis() + OnePeriod;
            HasTimerStarted = true;
        }
        if (TimeTarget <= millis()){
            dropCupState = OPEN_CLAW;
            HasTimerStarted = false;
        }  
        break;
    case OPEN_CLAW:
        openClaw();
        if (!HasTimerStarted){
            TimeTarget = millis() + OnePeriod;
            HasTimerStarted = true;
        }
        if (TimeTarget <= millis()){
            dropCupState = OPEN_CLAW;
            HasTimerStarted = false;
        }  
        break;
    case TEST_FINISHED:
        break;
    }
}
bool isDropCupActionFinished(){
    return dropCupState == TEST_FINISHED;
}

