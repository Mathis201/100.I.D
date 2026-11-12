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
#include <../lib/Grille/Position/PID_follow_line.h>
#include <../lib/Grille/Position/Position_Grille.h>


#define OUTPUT_LED_PIN 53

/*
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les variables globales
*/
void setup() {
    BoardInit();
    initLineSensor();    

}
// Fonctions de boucle infinie
// //  -> Se fait appeler perpetuellement suite au "setup"
// */
void loop() {

    if (ROBUS_IsBumper(REAR))
    {
        while (true)
        {
           Go_To_Position_XY(5, 3);
        }
    }
}
