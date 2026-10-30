#include "line_sensor.h"
#include "Arduino.h"
#include <LibRobus.h>

#define LINE_SENSOR_LEFT_PIN A7
#define LINE_SENSOR_MIDDLE_PIN A6
#define LINE_SENSOR_RIGHT_PIN A5

void initLineSensor() {
    pinMode(LINE_SENSOR_LEFT_PIN, INPUT);
    pinMode(LINE_SENSOR_MIDDLE_PIN, INPUT);
    pinMode(LINE_SENSOR_RIGHT_PIN, INPUT);
}

int getNormalizedLineReading() {
    Serial.print("Left: ");
    Serial.print(analogRead(LINE_SENSOR_LEFT_PIN));
    Serial.print(", Middle: ");
    Serial.print(analogRead(LINE_SENSOR_MIDDLE_PIN));
    Serial.print(", Right: ");
    Serial.println(analogRead(LINE_SENSOR_RIGHT_PIN));

    float valeurGauche = analogRead(LINE_SENSOR_LEFT_PIN);
    float valeurMilieu = analogRead(LINE_SENSOR_MIDDLE_PIN);
    float valeurDroite = analogRead(LINE_SENSOR_RIGHT_PIN);

    static int MaxGauche = 0;
    static int MaxMilieu = 0;
    static int MaxDroite = 0;
    static int MinGauche = valeurGauche;
    static int MinMilieu = valeurMilieu;
    static int MinDroite = valeurDroite;

    if (valeurGauche > MaxGauche){
        MaxGauche = valeurGauche;
           
    }

    if (valeurMilieu > MaxMilieu) {
        MaxMilieu = valeurMilieu;
     
    }
    if (valeurDroite > MaxDroite) {
        MaxDroite = valeurDroite;
         
        }
    if (valeurGauche < MinGauche){
        MinGauche = valeurGauche;
        

    }
     if (valeurMilieu < MinMilieu){
        MinMilieu = valeurMilieu;
        
    }

     if (valeurDroite < MinDroite){
        MinDroite = valeurDroite;
        
        
    }
    if(ROBUS_IsBumper(LEFT)) {
         Serial.print("Valeur max Gauche est : ");
    Serial.println(MaxGauche);
    Serial.print("Valeur max Milieu est : ");
    Serial.println(MaxMilieu);
    Serial.print("Valeur max Droite est : ");
    Serial.println(MaxDroite);
    Serial.print("Valeur min Gauche est: ");
    Serial.println(MinGauche);
    Serial.print("Valeur min Milieu est: ");
    Serial.println(MinMilieu);
    Serial.print("Valeur min Droit est: ");
    Serial.println(MinDroite);
    }
   




    return 0;



}
bool IsLineDetected(){
    return false;



}