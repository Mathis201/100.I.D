#include "green.h"
#include "capteurs/line_sensor.h"
#include "motors/motor_management.h"


enum GreenStates {
    AVANCER,
    AVANCERSANSLIGNE,
    RETROUVERLIGNE,
    TEST_FINISHED
};

GreenStates CurrentsStates

void initGreenAction() {
    CurrentsStates = AVANCER;
}

void updateGreenAction() {
    switch (CurrentsStates)
    {
    case AVANCER:
        Adapt_Curve();
        if (IsLineDetected()) {
            requestStraightDistance(750);
            CurrentsStates = AVANCERSANSLIGNE;   
        }
    
        break;
    
    case AVANCERSANSLIGNE:
        updateMotorProfiles();
        if (areProfilesDone()) {
            CurrentsStates = TEST_FINISHED;
        }

        break;

    case RETROUVERLIGNE:
        if (IsLineDetected()){
            requestStraightDistance(50);
            updateMotorProfiles;
        }

        if (IsLineDetected()){
            CurrentsStates = TEST_FINISHED;
        }




        break;


    default:
        break;
    }
}

bool isGreenActionFinished() {
    CurrentsStates=TEST_FINISHED;
}