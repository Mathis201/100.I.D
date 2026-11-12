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
            Follow_line(ton::white);
            if (ROBUS_IsBumper(RIGHT))
            {   
                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);
                break;
            }
            
        }
        
        
    }
    if (ROBUS_IsBumper(LEFT))
    {

        while (true)
            {
                Follow_line(ton::black);
                if (ROBUS_IsBumper(RIGHT))
                {   
                    MOTOR_SetSpeed(RIGHT, 0);
                    MOTOR_SetSpeed(LEFT, 0);
                    break;
                }
                
            }
    }
}
