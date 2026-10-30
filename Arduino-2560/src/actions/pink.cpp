#include "pink.h"
#include <LibRobus.h>
#include "capteurs/capteur_distance_ir.h"

float vitesse = 0.15; // VOIR LA VALEUR DE VITESSE À METTRE ICI


enum PinkStates {
    TURN_IN_PLACE,
    MOVE_TO_QUILLE,
    RETURN_TO_LINE,
    TEST_FINISHED
};

PinkStates pinkstate;

static long ticksDepartGauche;
static long ticksDepartDroite;

void initPinkAction() {
    pinkstate = TURN_IN_PLACE;

    ticksDepartGauche = ENCODER_Read(LEFT);
    ticksDepartDroite = ENCODER_Read(RIGHT);
}

void updatePinkAction() {
    switch (pinkstate) {
    case TURN_IN_PLACE:
        //TODO: Tourner sur place, pas trop rapidement
        float distance_cm = detecter_distance(0); //capteur IR 0
        if (distance_cm <= 50)
        {
            pinkstate = MOVE_TO_QUILLE;
            MOTOR_SetSpeed(LEFT/RIGHT, 0);  //Regarder si on met une décélération
        }
        break;
    case MOVE_TO_QUILLE: // sinon ça pourrait être case QUILLE_DETECTED, puis l'approche de la quille va suivre
        // TODO: avancer vers la quille
        break;
    case TEST_FINISHED:
        // Retrouver le parcours, donc revenir en arrière, et dans la bonne direction
        // Faire le nb de ticks inverse (pour orientation et déplacement)?
        break;
    }
}

bool isPinkActionFinished() {
    return pinkstate == TEST_FINISHED;
}
