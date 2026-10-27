
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
    return 30.0;
}
    

bool StartRotation = false;


void yellow() {
    switch(CurrentStates) {
        case WAIT_FOR_WALL:
        // Lire la distance du capteur:

        // Convertir la lecture en cm

        // J'ai inveter une distance du mur pour programmer
        float distance = lireDistanceMur();

        if (distance <= 15){

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

        if (abs(impulsionLeft) >= 1000 &&
         abs(impulsionRight) >= 1000)
        {
            MOTOR_SetSpeed(LEFT,0);
            MOTOR_SetSpeed(RIGHT,0);

            StartRotation = false;
            CurrentStates = FORWARD_1;
        }

        break;

    }

}
