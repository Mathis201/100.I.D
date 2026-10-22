#include <LibRobus.h>

int vitesse = 0.15;

enum PinkStates{
    TURN_IN_PLACE,
    MOVE_TO_QUILLE,
    RETURN_TO_LINE
};

PinkStates pinkstate = TURN_IN_PLACE;

void pink(){
    switch (pinkstate) {
        case TURN_IN_PLACE:
        MOTOR_SetSpeed(LEFT, vitesse); //VOIR LA VALEUR DE VITESSE À METTRE ICI
        MOTOR_SetSpeed(RIGHT, -vitesse);
        
        //Lecture du capteur de proximité
        //Quand on va détecter la quille, MOTOR_SetSpeed(LEFT/RIGHT, 0)
        //pinkstate
        break;
        case MOVE_TO_QUILLE:  //sinon ça pourrait être case QUILLE_DETECTED, puis l'approche de la quille va suivre
        //avancer vers la quille
        break;
        case RETURN_TO_LINE:
        //Retrouver le parcours, donc revenir en arrière, et dans la bonne direction
        //Faire le nb de ticks inverse (pour orientation et déplacement)?
    }
}