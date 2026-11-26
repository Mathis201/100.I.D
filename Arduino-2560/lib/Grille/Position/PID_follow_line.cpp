#include "PID_follow_line.h"
#include <Arduino.h>
#include <LibRobus.h>

#define LINE_SENSOR_LEFT_PIN A7
#define LINE_SENSOR_MIDDLE_PIN A6
#define LINE_SENSOR_RIGHT_PIN A5

void initLineSensor()
{
    pinMode(LINE_SENSOR_LEFT_PIN, INPUT);
    pinMode(LINE_SENSOR_MIDDLE_PIN, INPUT);
    pinMode(LINE_SENSOR_RIGHT_PIN, INPUT);
}

Capteurs Binary_Conversion()
{

    Capteurs Valeurs;

    float V_Left = analogRead(LINE_SENSOR_LEFT_PIN);
    float V_Middle = analogRead(LINE_SENSOR_MIDDLE_PIN);
    float V_Right = analogRead(LINE_SENSOR_RIGHT_PIN);

    float Blanc_Left = 700;
    float Blanc_Middle = 685;
    float Blanc_Right = 740;
    float Bleu_Left = 790;
    float Bleu_Middle = 780;
    float Bleu_Right = 805;
    float Noir_Left = 915;
    float Noir_Middle = 930;
    float Noir_Right = 970;

    float Left;
    float Middle;
    float Right;


    int Dif_Left = V_Left - Bleu_Left;
    int Dif_Middle = V_Middle - Bleu_Middle;
    int Dif_Right = V_Right - Bleu_Right;

    // if (ton == white)
    // {
    //     Valeurs.C_Left = constrain((Dif_Left) / (Bleu_Left - Blanc_Left), -1, 0);
    //     Valeurs.C_Middle = constrain((Dif_Middle) / (Bleu_Middle - Blanc_Middle), -1, 0);
    //     Valeurs.C_Right = constrain((Dif_Right) / (Bleu_Right - Blanc_Right), -1, 0);
    // }

    // if (ton == black)
    // {
    //     Valeurs.C_Left = constrain((Dif_Left) / (Noir_Left - Bleu_Left), 0, 1);
    //     Valeurs.C_Middle = constrain((Dif_Middle) / (Noir_Middle - Bleu_Middle), 0, 1);
    //     Valeurs.C_Right = constrain((Dif_Right) / (Noir_Right - Bleu_Right), 0, 1);
    // }

    if (V_Left <= Bleu_Left)
    {
        Left = constrain((Dif_Left) / (Bleu_Left - Blanc_Left), -1, 1);
    }
    else if (V_Left > Bleu_Left)
    {
        Left = constrain((Dif_Left) / (Noir_Left - Bleu_Left), -1, 1);
    }


    if (V_Middle <= Bleu_Middle)
    {
        Middle = constrain((Dif_Middle) / (Bleu_Middle - Blanc_Middle), -1, 1);
    }
    else if (V_Middle > Bleu_Middle)
    {
        Middle = constrain((Dif_Middle) / (Noir_Middle - Bleu_Middle), -1, 1);
    }
    

    if (V_Right <= Bleu_Right)
    {
        Right = constrain((Dif_Right) / (Bleu_Right - Blanc_Right), -1, 1);
    }
    else if (V_Right > Bleu_Right)
    {
        Right = constrain((Dif_Right) / (Noir_Right - Bleu_Right), -1, 1);
    }
    
    


    Valeurs.C_Left = Left;
    Valeurs.C_Middle = Middle;
    Valeurs.C_Right = Right;

    return Valeurs;
}

//_______________________________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________________________

unsigned long Last_Time = 0;
float Past_Error = 0;
float KD_N = 1;
float KP_N = .1;
float KD_B = 1;
float KP_B = .1;
 
float Capteurs_Left;
float Capteurs_Middle;
float Capteurs_Right;

float base_speed = .25;
float ponderation = .3;
float Live_Error = 0;
float Deriv = 0;
bool First = true;
unsigned long Now = millis();


void Follow_line(int ligne)
{
    Capteurs Valeurs = Binary_Conversion();

    Capteurs_Left = Valeurs.C_Left;
    Capteurs_Middle = Valeurs.C_Middle;
    Capteurs_Right = Valeurs.C_Right;

    if (ligne == white)
    {

        Capteurs_Left = -Capteurs_Left;
        Capteurs_Middle = -Capteurs_Middle;
        Capteurs_Right = -Capteurs_Right;

        follow_line_calc(white);

    }

    else if (ligne == black)
    {

        follow_line_calc(black);
    }

    Last_Time = Now;
    First = false;
}






int getNormalizedLineReading()
{
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

    if (valeurGauche > MaxGauche)
    {
        MaxGauche = valeurGauche;
    }

    if (valeurMilieu > MaxMilieu)
    {
        MaxMilieu = valeurMilieu;
    }
    if (valeurDroite > MaxDroite)
    {
        MaxDroite = valeurDroite;
    }
    if (valeurGauche < MinGauche)
    {
        MinGauche = valeurGauche;
    }
    if (valeurMilieu < MinMilieu)
    {
        MinMilieu = valeurMilieu;
    }

    if (valeurDroite < MinDroite)
    {
        MinDroite = valeurDroite;
    }
    if (ROBUS_IsBumper(LEFT))
    {
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






void Live_Error_Calc(int side){

    if (side == RIGHT)
    {
        Live_Error = Capteurs_Right * ponderation + (1 - Capteurs_Middle);
    }
    else if (side == LEFT)
    {
        Live_Error = Capteurs_Left * ponderation + (1 - Capteurs_Middle);
    }
}


void Deriv_Calc_PID(){

    Now = millis();

    if ((Now - Last_Time != 0) && (Now - Last_Time < 250) && (First != true))
    {
        Deriv = (Live_Error - Past_Error) / (Now - Last_Time);
    }
}


float Motor_Calc(int ton){

    if (ton = white)
    {
        return (Live_Error * KP_B + Deriv * KD_B);
    }
    else if (ton = black)
    {
        return (Live_Error * KP_N + Deriv * KD_N);
    }
    
    

}




void follow_line_calc(int ton){

    if (Capteurs_Left < -.9 && Capteurs_Right < -.9)
        {
            MOTOR_SetSpeed(RIGHT, base_speed);
            MOTOR_SetSpeed(LEFT, base_speed);
        }


    else if (Capteurs_Left < Capteurs_Right)
        {
            Live_Error_Calc(RIGHT);
            
            Deriv_Calc_PID();
            
            MOTOR_SetSpeed(LEFT, base_speed + Motor_Calc(ton));
            MOTOR_SetSpeed(RIGHT, base_speed - Motor_Calc(ton));
            Past_Error = Live_Error;
        }


    else if (Capteurs_Right < Capteurs_Left)
        {
            Live_Error_Calc(LEFT);

            Deriv_Calc_PID();

            MOTOR_SetSpeed(LEFT, base_speed - Motor_Calc(ton));
            MOTOR_SetSpeed(RIGHT, base_speed + Motor_Calc(ton));
            Past_Error = Live_Error;
        }

    else
        {
            MOTOR_SetSpeed(RIGHT, base_speed);
            MOTOR_SetSpeed(LEFT, base_speed);
        }
}