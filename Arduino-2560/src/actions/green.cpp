#include "green.h"
#include "capteurs/line_sensor.h"
#include "motors/motor_management.h"
#include "Arduino.h"


enum GreenStates {
    AVANCER,
    AVANCERSANSLIGNE,
    RETROUVERLIGNE,
    Tourner_1,
    Avancer_1,
    Tourner_2,
    Avancer_2,
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
            if (IsLineDetected(1)){
                CurrentsStates = TEST_FINISHED;

            }
            else {
                CurrentsStates = Tourner_1;
            }          
            
        }

        break;
    }

    case Tourner_1:{
        requestTurnDegrees(90);
        updateMotorProfiles();
        if (areProfilesDone()) {
            if (IsLineDetected(1)){
                CurrentsStates = TEST_FINISHED;

            }
            else {
                CurrentsStates = Avancer_1;
            }          
            
        }

        break;

    }
     case Avancer_1:{
        requestStraightDistance(200);
        updateMotorProfiles();
        if (areProfilesDone()) {
            if (IsLineDetected(1)){
                CurrentsStates = TEST_FINISHED;

            }
            else {
                CurrentsStates = Tourner_2;
            }          
            
        }

        break;

    }

     case Tourner_2:{
        requestTurnDegrees(180);
        updateMotorProfiles();
        if (areProfilesDone()) {
            if (IsLineDetected(1)){
                CurrentsStates = TEST_FINISHED;

            }
            else {
                CurrentsStates = Avancer_2;
            }         
            
        }

        break;

    }
      case Avancer_2:{
        requestStraightDistance(40);
        updateMotorProfiles();
        if (areProfilesDone()) {
            if (IsLineDetected(1)){
                CurrentsStates = TEST_FINISHED;

            }
            else {
                CurrentsStates = Tourner_1;
            }         
            
        }

        break;

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