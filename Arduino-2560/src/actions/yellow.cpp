#include "yellow.h"
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

// Liste de constante pour le programme
bool StartRotation = false;        // Condition pour la rotation
bool StartForward = false;         // Condition pour avancer
const int32_t Impulsion_90 = 1984; // Calcul de combien d'impulsion des roues pour faire 90 degrés
const int SEUIL_IR = 500;          // Valeur numerique donner par arduino qu'on dit au robot d'Arreter devant le mur
const float kp = 0.0001;
const int32_t impulsion_forward_1 = 4010;// imulsion pour avancer le long du mur 
const int32_t impulsion_forward_2 = 4010; // impulsion pour avancer sur le cote du mur
RobotStates CurrentStates = WAIT_FOR_WALL; // etat initial


uint16_t LireDistanceMur(){
    return ROBUS_ReadIR(0);


}
void initYellowAction() {
    CurrentStates = WAIT_FOR_WALL;
    StartForward = false;
    StartRotation = false; 

}

bool isYellowActionFinished() {
    return CurrentStates == FINISHED;
}

// TODO: merge la logique de sam dans les fonctions prédéfinies


void updateYellowAction() {
    switch (CurrentStates) {
    case WAIT_FOR_WALL:{
        // Lire la distance du capteur:

        uint16_t valeurIR = LireDistanceMur();

        if (valeurIR >= SEUIL_IR) { // Condition pour arreter devant le mur

            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            CurrentStates = TURN_1;
        } else {
            // Continue à suivre la ligne
        }
        break;
        }

    case TURN_1:{ // Premier virage de 90 degrés

        if (StartRotation == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartRotation = true;
        }

        MOTOR_SetSpeed(LEFT, 0.5); // Rotation de 90 degrés
        MOTOR_SetSpeed(RIGHT, -0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        if (abs(impulsionLeft) >= Impulsion_90 && // Condition pour arreter de tourner
            abs(impulsionRight) >= Impulsion_90)  // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FORWARD_1;
        }

        break;
        }

    case FORWARD_1:{ // Avancement du robot
        if (StartForward == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartForward = true;
        }

        MOTOR_SetSpeed(LEFT, 0.5);
        MOTOR_SetSpeed(RIGHT, 0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        int32_t erreur = impulsionLeft - impulsionRight; // PID pour avancer
        float correction = kp * erreur;
        float vitesseLeft = 0.5 - correction;
        float vitesseRight = 0.5 + correction;

        MOTOR_SetSpeed(LEFT, vitesseLeft); // correction pour aller droit
        MOTOR_SetSpeed(RIGHT, vitesseRight);

        if (impulsionLeft >= impulsion_forward_1 && // Conidtion pour arreter le moteur d'avancer
            impulsionRight >= impulsion_forward_1) {

            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartForward = false;
            CurrentStates = TURN_2;
        }
        break;
        }

    case TURN_2:{ // Deuxième tournant pour contourner le mur
        if (StartRotation == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartRotation = true;
        }

        MOTOR_SetSpeed(LEFT, -0.5); // Rotation de 90 degrés à gauche
        MOTOR_SetSpeed(RIGHT, 0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        if (abs(impulsionLeft) >= Impulsion_90 && // Condition pour arreter de tourner
            abs(impulsionRight) >= Impulsion_90)  // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FORWARD_2;
        }

        break;
        }


    case FORWARD_2:{
        if (StartForward == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartForward = true;
        }

        MOTOR_SetSpeed(LEFT, 0.5);
        MOTOR_SetSpeed(RIGHT, 0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        int32_t erreur = impulsionLeft - impulsionRight; // PID pour avancer
        float correction = kp * erreur;
        float vitesseLeft = 0.5 - correction;
        float vitesseRight = 0.5 + correction;

        MOTOR_SetSpeed(LEFT, vitesseLeft); // correction pour aller droit
        MOTOR_SetSpeed(RIGHT, vitesseRight);

        if (impulsionLeft >= impulsion_forward_2 && // Conidtion pour arreter le moteur d'avancer
            impulsionRight >= impulsion_forward_2) {

            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartForward = false;
            CurrentStates = TURN_3;
        }
        break;
        }
        
    case TURN_3:{
        if (StartRotation == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartRotation = true;
        }

        MOTOR_SetSpeed(LEFT, -0.5); // Rotation de 90 degrés à gauche
        MOTOR_SetSpeed(RIGHT, 0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        if (abs(impulsionLeft) >= Impulsion_90 && // Condition pour arreter de tourner
            abs(impulsionRight) >= Impulsion_90)  // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FORWARD_3;
        }

        break;

    }
    
    case FORWARD_3:{

    }


    case TURN_4:{
        if (StartRotation == false) {
            ENCODER_ReadReset(LEFT);
            ENCODER_ReadReset(RIGHT);

            StartRotation = true;
        }

        MOTOR_SetSpeed(LEFT, 0.5); // Rotation de 90 degrés
        MOTOR_SetSpeed(RIGHT, -0.5);

        int32_t impulsionLeft = ENCODER_Read(LEFT);
        int32_t impulsionRight = ENCODER_Read(RIGHT);

        if (abs(impulsionLeft) >= Impulsion_90 && // Condition pour arreter de tourner
            abs(impulsionRight) >= Impulsion_90)  // Condition pour arreter de tourner
        {
            MOTOR_SetSpeed(LEFT, 0);
            MOTOR_SetSpeed(RIGHT, 0);

            StartRotation = false;
            CurrentStates = FINISHED;
        }

        break;

    }

    }

}
