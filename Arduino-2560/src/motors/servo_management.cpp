#include "servo_management.h"
#include <LibRobus.h>


const int indexArm = 0; //Servo qui monte et descend la pince
const int indexClaw = 1; //Servo qui ouvre et ferme la pince


void initServo(){ //active les servos, doit etre appelé à l'initialisation du robot
    SERVO_Enable(indexArm);
    SERVO_Enable(indexClaw);
}

void disableServo(){ //désactive les servos, doit être appelé en fin de programme 
    SERVO_Disable(indexArm);
    SERVO_Disable(indexClaw);
}

void openClaw(){ //ouvre la pince 
    SERVO_SetAngle(indexClaw, 0);
}

void raiseArm(){ //monte le bras de 90deg
    SERVO_SetAngle(indexArm, 90);
}

void closeClaw(){ //ferme la pince 
    SERVO_SetAngle(indexClaw, 130);
}

void lowerArm(){ //baisse le bras 
    SERVO_SetAngle(indexArm, 0);
}

void pickUpCup(){ //prend le verre (ferme puis lève)
    closeClaw();
    raiseArm();
}

void dropCup(){ //drop le verre (baisse puis ouvre)
    lowerArm();
    openClaw();
}