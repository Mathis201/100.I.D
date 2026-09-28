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

#define OUTPUT_LED_PIN 53

/*
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les variables globales
*/
void setup() {
    BoardInit();
    Serial1.begin(BAUD_RATE_SERIAL0);

    // initialisation
    pinMode(OUTPUT_LED_PIN, OUTPUT);
    digitalWrite(OUTPUT_LED_PIN, 0);
}

/*
Fonctions de boucle infinie
 -> Se fait appeler perpetuellement suite au "setup"
*/
void loop() {
    if (Serial1.available() > 0) {
        int newVal = Serial1.parseInt();
        Serial.println(newVal);
        digitalWrite(OUTPUT_LED_PIN, newVal);
    }
}
