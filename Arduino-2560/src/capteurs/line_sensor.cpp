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

Capteurs Binary_Conversion() {

    Capteurs Valeurs;

    float V_Left = analogRead(LINE_SENSOR_LEFT_PIN);
    float V_Middle = analogRead(LINE_SENSOR_MIDDLE_PIN);
    float V_Right = analogRead(LINE_SENSOR_RIGHT_PIN);

    float Max_Left = 641;
    float Max_Middle = 559;
    float Max_Right = 663;
    float Min_Left = 39;
    float Min_Middle = 31;
    float Min_Right = 54;

    Valeurs.C_Left = constrain((V_Left - Min_Left) / (Max_Left - Min_Left), 0, 1);
    Valeurs.C_Middle = constrain((V_Middle - Min_Middle) / (Max_Middle - Min_Middle), 0, 1);
    Valeurs.C_Right = constrain((V_Right - Min_Right) / (Max_Right - Min_Right), 0, 1);

    return Valeurs;
}

unsigned long Last_Time = 0;
float Past_Error = 0;

void Adapt_Curve() {

    float KD = 0.1;
    float KP = .2;

    float base_speed = .25;
    float ponderation = .4;

    float Live_Error = 0;
    float Deriv = 0;
    bool First = true;

    Capteurs Valeurs = Binary_Conversion();

    unsigned long Now = millis();

    // if (Valeurs.C_Middle>.90)
    // {
    //     MOTOR_SetSpeed(RIGHT, base_speed);
    //     MOTOR_SetSpeed(LEFT, base_speed+0.01);
    // }

    if (Valeurs.C_Left > Valeurs.C_Right) {
        Live_Error = (Valeurs.C_Left * ponderation + (1 - Valeurs.C_Middle));
        if (Now - Last_Time != 0 || Now - Last_Time < 500 || First != true) {
            Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
        }

        MOTOR_SetSpeed(LEFT, base_speed - (Live_Error * KP + Deriv * KD));
        MOTOR_SetSpeed(RIGHT, base_speed + (Live_Error * KP + Deriv * KD));
        Past_Error = Live_Error;
    }

    else if (Valeurs.C_Right > Valeurs.C_Left) {
        Live_Error = (Valeurs.C_Right * ponderation + (1 - Valeurs.C_Middle));
        if (Now - Last_Time != 0 || Now - Last_Time < 500 || First != true) {
            Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
        }
        MOTOR_SetSpeed(LEFT, base_speed + (Live_Error * KP + Deriv * KD));
        MOTOR_SetSpeed(RIGHT, base_speed - (Live_Error * KP + Deriv * KD));
        Past_Error = Live_Error;
    }

    else {
        MOTOR_SetSpeed(RIGHT, base_speed);
        MOTOR_SetSpeed(LEFT, base_speed);
    }

    Last_Time = Now;
    First = false;

    // if (Valeurs.C_Middle>.90)
    // {
    //     MOTOR_SetSpeed(LEFT, base_speed+.01);
    //     MOTOR_SetSpeed(RIGHT, base_speed);
    // }

    // else if (Valeurs.C_Left>Valeurs.C_Right)
    // {
    // MOTOR_SetSpeed(LEFT, base_speed-(Valeurs.C_Left+(1-Valeurs.C_Middle))*KP);
    // MOTOR_SetSpeed(RIGHT, base_speed+(Valeurs.C_Left+(1-Valeurs.C_Middle))*KP);

    // }

    // else if (Valeurs.C_Right>Valeurs.C_Left)
    // {
    // MOTOR_SetSpeed(LEFT, base_speed+(Valeurs.C_Left+(1-Valeurs.C_Middle))*KP);
    // MOTOR_SetSpeed(RIGHT, base_speed-(Valeurs.C_Left+(1-Valeurs.C_Middle))*KP);
}

bool IsLineDetected(int nbvoulu) {

    int nbcapteurs = 0;
    Capteurs Valeurs = Binary_Conversion();

    if (Valeurs.C_Left >= 0.4) {
        nbcapteurs = nbcapteurs + 1;
    }
    if (Valeurs.C_Middle >= 0.4) {
        nbcapteurs = nbcapteurs + 1;
    }
    if (Valeurs.C_Right >= 0.4) {
        nbcapteurs = nbcapteurs + 1;
    }
    Serial.print("Gauche: ");
    Serial.println(Valeurs.C_Left);
    Serial.print("Milieu: ");
    Serial.println(Valeurs.C_Middle);
    Serial.print("Droite: ");
    Serial.println(Valeurs.C_Right);

    if (nbcapteurs < nbvoulu) {
        return false;
    } else {
        return true;
    }
}