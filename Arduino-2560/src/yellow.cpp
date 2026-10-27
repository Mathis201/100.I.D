
#include <Arduino.h>
#include <LibRobus.h>


enum RobotStates {
    WAIT_FOR_WALL,
    TURN_1,
    FORWARD_1,
    TURN_2,
    FORWARD_2,
    TURN_3,
    FORWARD_3,
    TURN_4,
    FINISHED
};

RobotStates CurrentStates = WAIT_FOR_WALL;

float lireDistanceMur() {
    uint16_t valeurIR = ROBUS_ReadIR(0);
}
    

bool StartRotation = false;
const int32_t Impulsion_90 = 1984;
const int SEUIL_IR = 500;



void yellow() {
    switch(CurrentStates) {
        case WAIT_FOR_WALL:
        // Lire la distance du capteur:

        uint16_t valeurIR = ROBUS_ReadIR(0);

        if (valeurIR >= SEUIL_IR){

            MOTOR_SetSpeed(LEFT,0);
            MOTOR_SetSpeed(RIGHT,0);

            CurrentStates = TURN_1;
        }
        else {
            // Continue à suivre la ligne
        }
        break;

        case TURN_1:

        if (StartRotation == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartRotation = true;
        }

        MOTOR_SetSpeed(LEFT,0.5);
        MOTOR_SetSpeed(RIGHT,-0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        if (abs(impulsionLeft) >= Impulsion_90 &&
         abs(impulsionRight) >= Impulsion_90)
        {
            MOTOR_SetSpeed(LEFT,0);
            MOTOR_SetSpeed(RIGHT,0);

            StartRotation = false;
            CurrentStates = FORWARD_1;
        }

        break;

        case FORWARD_1:
        

    }

}
