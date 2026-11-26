#include "servo_management.h"
#include <LibRobus.h>

constexpr int indexDoor = 0;  // Servo qui ouvre la porte de la rampe
constexpr int indexPlate = 1; // Servo pour choisir quel pastille rouge ou blanc et porter à la rampe

constexpr int plateWhiteAngle = 10;
constexpr int plateMiddleAngle = 90;
constexpr int plateRedAngle = 170;

void initServo() {
    SERVO_Enable(indexDoor);
    SERVO_Enable(indexPlate);
    // openDoor();
}

void openDoor() { // Ouvre la porte pour faire tomber la pastille
    // SERVO_SetAngle(0,90); // je dois mettre le servo en position zero avant de set un angle
}

void closeDoor() { // Ferme la porte
    // SERVO_SetAngle(0,0); // Meme chose doit trouver le set point
}

void sendPlateToMiddle() {
    SERVO_SetAngle(indexPlate, plateMiddleAngle);
}

void sendPlateToWhite() {
    SERVO_SetAngle(indexPlate, plateWhiteAngle);
}

void sendPlateToRed() {
    SERVO_SetAngle(indexPlate, plateRedAngle);
}