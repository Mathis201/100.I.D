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
#include <MINI_MP3/audio.h>
#define OUTPUT_LED_PIN 53

/*
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les variables globales
*/
void setup() {
    Serial.begin(9600);
    audio_init();
    Serial.print("Pistes sur la carte SD: ");
    Serial.println(audio_nb_pistes());
}

/*
Fonctions de boucle infinie
 -> Se fait appeler perpetuellement suite au "setup"
*/
void loop() {
audio_jouer(Rate);
delay(10000);
audio_jouer(Touche);
delay(10000);
audio_jouer(Coule);
delay(10000);
}
