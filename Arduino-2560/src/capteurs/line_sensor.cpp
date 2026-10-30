#include "line_sensor.h"
#include "Arduino.h"

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

    int MaxGauche = 0;
    int MaxMilieu = 0;
    int MaxDroite = 0;

    if (valeurGauche > MaxGauche){
        MaxGauche = valeurGauche;
        Serial.print("Valeur max Gauche est : ");
        Serial.println(MaxGauche);
    
    }

    if (valeurMilieu > MaxMilieu) {
        MaxMilieu = valeurMilieu;
        Serial.print("Valeur max Milieu est : ");
        Serial.println(MaxMilieu);
    }
    if (valeurDroite > MaxDroite) {
        MaxDroite = valeurDroite;
        Serial.print("Valeur max Droite est : ");
        Serial.println(MaxDroite); 
        }

    return 0;



}
bool IsLineDetected(){
    return false;



}