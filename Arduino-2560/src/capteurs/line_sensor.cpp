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






struct Capteurs
{
    float C_Left;
    float C_Middle;
    float C_Right;
};

Capteurs Binary_Conversion(){

    Capteurs Valeurs;

    float V_Left = analogRead(LINE_SENSOR_LEFT_PIN);
    float V_Middle = analogRead(LINE_SENSOR_MIDDLE_PIN);
    float V_Right = analogRead(LINE_SENSOR_RIGHT_PIN);

    float Max_Left = 708;
    float Max_Middle = 508;
    float Max_Right = 507;
    float Min_Left = 33;
    float Min_Middle = 26;
    float Min_Right = 25;
    

    Valeurs.C_Left = (abs(V_Left-Min_Left))/(Max_Left-Min_Left);
    Valeurs.C_Middle = (abs(V_Middle-Min_Middle))/(Max_Middle-Min_Middle);
    Valeurs.C_Right = (abs(V_Right-Min_Right))/(Max_Right-RIGHT);
    

    return Valeurs;
}

void Adapt_Curve(){

 Capteurs Valeurs = Binary_Conversion();
    
if (Valeurs.C_Left>Valeurs.C_Right)
{
MOTOR_SetSpeed(LEFT, .5-Valeurs.C_Left/2);
MOTOR_SetSpeed(RIGHT, .5/(Valeurs.C_Middle+.001));
    
}

else if (Valeurs.C_Right>Valeurs.C_Left)
{
MOTOR_SetSpeed(LEFT, 0.5/(Valeurs.C_Middle+0.001));
MOTOR_SetSpeed(RIGHT, 0.5-Valeurs.C_Right/2);
    
}
}






bool IsLineDetected(){
    return false;



}