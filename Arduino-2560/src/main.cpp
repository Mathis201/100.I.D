/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/*
Inclure les librairies de functions que vous voulez utiliser
*/
#include <LibRobus.h>

/*
Variables globales et defines
 -> defines...
 -> L'ensemble des fonctions y ont acces
*/
enum RobotStates {
    WAIT_FOR_START,
    FOLLOW_LINE,
    DO_GREEN,
    DO_BLUE,
    DO_YELLOW,
    DO_ORANGE,
    DO_PINK,
    DROP_CUP,
    FINISHED
};

enum PinkStates {
    TURN_IN_PLACE,
    MOVE_TO_QUILLE,
    RETURN_TO_LINE
};

/*
Vos propres fonctions sont creees ici
*/

/*
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les varibbles globales
*/
void setup() {
    BoardInit();
}

/*
Fonctions de boucle infini
 -> Se fait appeler perpetuellement suite au "setup"
*/
void loop() {
}
