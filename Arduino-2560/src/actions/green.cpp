#include "green.h"
#include "capteurs/line_sensor.h"
#include "motors/motor_management.h"
#include "Arduino.h"


enum GreenStates {
    AVANCER,
    AVANCERSANSLIGNE,
    RETROUVERLIGNE,
    TEST_FINISHED
};

GreenStates CurrentsStates;

void initGreenAction() {
    CurrentsStates = AVANCER;
}

void updateGreenAction() {
    switch (CurrentsStates)
    {
    case AVANCER:{
        Adapt_Curve();
        if (IsLineDetected(3)) {
            requestStraightDistance(800);
            CurrentsStates = AVANCERSANSLIGNE;   
        }
    
        break;
    }
    
    case AVANCERSANSLIGNE:{
        updateMotorProfiles();
        if (areProfilesDone()) {
            CurrentsStates = TEST_FINISHED;
        }

        break;
    }

    case RETROUVERLIGNE:{
        // if (IsLineDetected(3)){
        //     requestStraightDistance(50);
        //     updateMotorProfiles;
        // }

        // if (IsLineDetected(1)){
        //     CurrentsStates = TEST_FINISHED;
        // }

        // break;

    }
    default:
        break;
    }
}

bool isGreenActionFinished() {
     if (CurrentsStates==TEST_FINISHED){
        return true;
     }
     else {
        return false;
     }
  
}