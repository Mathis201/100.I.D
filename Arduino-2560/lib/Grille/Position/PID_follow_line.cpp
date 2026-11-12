#include <LibRobus.h>
#include <Arduino.h>
#include "PID_follow_line.h"

#define LINE_SENSOR_LEFT_PIN A7
#define LINE_SENSOR_MIDDLE_PIN A6 
#define LINE_SENSOR_RIGHT_PIN A5



void initLineSensor() {
    pinMode(LINE_SENSOR_LEFT_PIN, INPUT);
    pinMode(LINE_SENSOR_MIDDLE_PIN, INPUT);
    pinMode(LINE_SENSOR_RIGHT_PIN, INPUT);
}



Capteurs Binary_Conversion(int ton) {

    Capteurs Valeurs;

    float V_Left = analogRead(LINE_SENSOR_LEFT_PIN);
    float V_Middle = analogRead(LINE_SENSOR_MIDDLE_PIN);
    float V_Right = analogRead(LINE_SENSOR_RIGHT_PIN);

    float Blanc_Left = 208;
    float Blanc_Middle = 54;
    float Blanc_Right = 266;
    float Bleu_Left = 487;
    float Bleu_Middle = 265;
    float Bleu_Right = 520;
    float Noir_Left = 792;
    float Noir_Middle = 723;
    float Noir_Right = 808;

    int Dif_Left = V_Left - Bleu_Left;
    int Dif_Middle = V_Middle - Bleu_Middle;
    int Dif_Right = V_Right - Bleu_Right;



    if (ton == white)
    {
        Valeurs.C_Left = constrain((Dif_Left) / (Bleu_Left - Blanc_Left), -1, 0);
        Valeurs.C_Middle = constrain((Dif_Middle) / (Bleu_Middle - Blanc_Middle), -1, 0);
        Valeurs.C_Right = constrain((Dif_Right) / (Bleu_Right - Blanc_Right), -1, 0);
    }

    if (ton == black)
    {
        Valeurs.C_Left = constrain((Dif_Left) / (Noir_Left-Bleu_Left), 0, 1);
        Valeurs.C_Middle = constrain((Dif_Middle) / (Noir_Middle - Bleu_Middle), 0, 1);
        Valeurs.C_Right = constrain((Dif_Right) / (Noir_Right-Bleu_Right), 0, 1);
    }

    return Valeurs;
}


//_______________________________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________________________




unsigned long Last_Time = 0;
float Past_Error = 0;

void Follow_line(int ligne) {

    float KD_N = 1;
    float KP_N = .1;
    float KD_B = 1;
    float KP_B = .1;

    float base_speed = .25;
    float ponderation = .4;
    float Live_Error = 0;
    float Deriv = 0;
    bool First = true;
    unsigned long Now = millis();

   

    Capteurs Valeurs = Binary_Conversion(ligne);

    if (ligne == white)
    {
        
        if (Valeurs.C_Left > .9 && Valeurs.C_Right > .9)
        {
                MOTOR_SetSpeed(RIGHT, base_speed);
                MOTOR_SetSpeed(LEFT, base_speed);
        }
        
        else if (Valeurs.C_Left < Valeurs.C_Right) {
                Live_Error = -(Valeurs.C_Left * ponderation - (1 + Valeurs.C_Middle));
                if (Now - Last_Time != 0 || Now - Last_Time < 250 || First != true) {
                    Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
                }

                MOTOR_SetSpeed(LEFT, base_speed - (Live_Error * KP_B + Deriv * KD_B));
                MOTOR_SetSpeed(RIGHT, base_speed + (Live_Error * KP_B + Deriv * KD_B));
                Past_Error = Live_Error;
            }

        else if (Valeurs.C_Right < Valeurs.C_Left) {
                Live_Error = -(Valeurs.C_Right * ponderation - (1 + Valeurs.C_Middle));
                if (Now - Last_Time != 0 || Now - Last_Time < 500 || First != true) {
                    Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
                }
                MOTOR_SetSpeed(LEFT, base_speed + (Live_Error * KP_B + Deriv * KD_B));
                MOTOR_SetSpeed(RIGHT, base_speed - (Live_Error * KP_B + Deriv * KD_B));
                Past_Error = Live_Error;
            }

        else {
                MOTOR_SetSpeed(RIGHT, base_speed);
                MOTOR_SetSpeed(LEFT, base_speed);
            }
    }


    else if (ligne == black)
    {

        if (Valeurs.C_Left < -.9 && Valeurs.C_Right < -.9)
        {
                MOTOR_SetSpeed(RIGHT, base_speed);
                MOTOR_SetSpeed(LEFT, base_speed);
        }

        else if (Valeurs.C_Left > Valeurs.C_Right) {
                Live_Error = (Valeurs.C_Left * ponderation + (1 - Valeurs.C_Middle));
                if (Now - Last_Time != 0 || Now - Last_Time < 500 || First != true) {
                    Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
                }

                MOTOR_SetSpeed(LEFT, base_speed - (Live_Error * KP_N + Deriv * KD_N));
                MOTOR_SetSpeed(RIGHT, base_speed + (Live_Error * KP_N + Deriv * KD_N));
                Past_Error = Live_Error;
            }

        else if (Valeurs.C_Right > Valeurs.C_Left) {
                Live_Error = (Valeurs.C_Right * ponderation + (1 - Valeurs.C_Middle));
                if (Now - Last_Time != 0 || Now - Last_Time < 500 || First != true) {
                    Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
                }
                MOTOR_SetSpeed(LEFT, base_speed + (Live_Error * KP_N + Deriv * KD_N));
                MOTOR_SetSpeed(RIGHT, base_speed - (Live_Error * KP_N + Deriv * KD_N));
                Past_Error = Live_Error;
            }

            else {
                MOTOR_SetSpeed(RIGHT, base_speed);
                MOTOR_SetSpeed(LEFT, base_speed);
            }
    }
    
    Last_Time = Now;
    First = false;
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

    static unsigned long SommeGauche = 0;
    static unsigned long SommeMilieu = 0;
    static unsigned long SommeDroite = 0;
    static int nombreFois = 0;
    static int MaxGauche = 0;
    static int MaxMilieu = 0;
    static int MaxDroite = 0;
    static int MinGauche = valeurGauche;
    static int MinMilieu = valeurMilieu;
    static int MinDroite = valeurDroite;

    SommeGauche += valeurGauche;
    SommeMilieu += valeurMilieu;
    SommeDroite += valeurDroite;
    nombreFois++;

    if (valeurGauche > MaxGauche) {
        MaxGauche = valeurGauche;
    }

    if (valeurMilieu > MaxMilieu) {
        MaxMilieu = valeurMilieu;
    }
    if (valeurDroite > MaxDroite) {
        MaxDroite = valeurDroite;
    }
    if (valeurGauche < MinGauche) {
        MinGauche = valeurGauche;
    }
    if (valeurMilieu < MinMilieu) {
        MinMilieu = valeurMilieu;
    }

    if (valeurDroite < MinDroite) {
        MinDroite = valeurDroite;
    }
    if (ROBUS_IsBumper(LEFT)) {
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
        unsigned long MoyenneGauche = (SommeGauche) / nombreFois;
        unsigned long MoyenneMilieu = (SommeMilieu) / nombreFois;
        unsigned long MoyenneDroite = (SommeDroite) / nombreFois;

        Serial.print("Moyenne Gauche:");
        Serial.println(MoyenneGauche);
        Serial.print("Moyenne Milieu:");
        Serial.println(MoyenneMilieu);
        Serial.print("Moyenne Droite:");
        Serial.println(MoyenneDroite);
    }

    return 0;
}
















