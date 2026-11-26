
#include "Position_Grille.h"
#include <LibRobus.h>
#include <Arduino.h>
#include "PID_follow_line.h"
#include <../../src/motors/motor_management.h>


// Variable globale

int X_NOW = 0;
int Y_NOW = 0;
int X_Target = 0;
int Y_Target = 0;
int Sens_robot = X_Axis_Pos;
int Action = How_much_turn;
unsigned long Last_time_line_detected = 0;
float pourcentage_ton_detect = .9;
int Next_Step = 0;
bool Already_Requested = false;
long aR;
long aL;

// -----------------------------------------------------------------------------------------

// Fonctions:

void Request_Reach_Position_XY(int Target_X, int Target_Y) // Reset la case target et remet Go_To_Position à la premiere etape
{

Action = How_much_turn;

X_Target = Target_X;
Y_Target = Target_Y;

}


bool Update_Reach_Position_XY() // Switch case
{

    switch (Action)
    {
    case How_much_turn:
        
        fonction_How_much_turn();

        break;

    case advance_5:

            if (Already_Requested != true)
            {
                requestStraightDistance(5);
                updateMotorProfiles();
                Already_Requested = true;
            }

            if (areProfilesDone())
            {
                Already_Requested = false;
                Action = Next_Step;
            }

        break;
    
    case Follow_line_straight:

                if (is_horizontal_detected() && (Wich_Line_Follow() == black))
                {
                    if (millis()-Last_time_line_detected >= 750)
                    {
                        Action = Horizontal_Detected;
                        Last_time_line_detected = millis();
                        break;
                    }
                    
                    
                }
                else if (is_vertical_detected() && (Wich_Line_Follow() == white))
                {
                    if (millis()-Last_time_line_detected >= 750)
                    {
                    Action = Vertical_Detected;
                    Last_time_line_detected = millis();
                    break;
                    }
                }


            Follow_line(Wich_Line_Follow());

            break;

    case Horizontal_Detected:

            if (Sens_robot == Y_Axis_Pos)
            {
                ++Y_NOW;
            }
            else if (Sens_robot == Y_Axis_Neg)
            {
                --Y_NOW;
            }
            if (Y_NOW == Y_Target)
            {
                Action = Finished;
                break;
            }
            
            Action = Follow_line_straight;
            
            break;

    case Vertical_Detected:

            if (Sens_robot == X_Axis_Pos)
            {
                ++X_NOW;
            }

            else if (Sens_robot == X_Axis_Neg)
            {
                --X_NOW;
            }
            
            if (X_NOW == X_Target && Y_NOW == Y_Target)
            {
                Action = Finished;
                break;
            }
            
            else if (X_NOW == X_Target)
            {

                Action = decelerate_To_0;
                break;
            }

            
            Action = Follow_line_straight;
            break;
        

    case TurnLeft:

            if (areProfilesDone() && (Already_Requested != true))
            {
                requestTurnDegrees(105);
                aR = ENCODER_Read(RIGHT);
                aL = ENCODER_Read(LEFT);
                updateMotorProfiles();
                Fonc_Orientation(-1);
                Already_Requested = true;
            }

            if (getAngleSince(aL,aR) >= 0.9*105)
            {
                Next_Step = Retrouver_ligne;

                if(is_horizontal_detected() || is_vertical_detected())
                {

                    Action = Follow_line_straight;
                    Already_Requested = false;

                }
            }
            
            break;

    case TurnRight:

            if (areProfilesDone() && (Already_Requested != true))
            {
                requestTurnDegrees(-105);
                aR = ENCODER_Read(RIGHT);
                aL = ENCODER_Read(LEFT);
                updateMotorProfiles();
                Fonc_Orientation(1);
                Already_Requested = true;
            }

            if (getAngleSince(aL,aR) <= 0.9*-105)
            {

                Next_Step = Retrouver_ligne;

                if(is_horizontal_detected() || is_vertical_detected())
                {

                    Action = Follow_line_straight;
                    Already_Requested = false;

                }
            }
            
            break;
    case U_Turn:

            if (areProfilesDone() && (Already_Requested != true))
            {
                requestTurnDegrees(-200);
                updateMotorProfiles();
                aR = ENCODER_Read(RIGHT);
                aL = ENCODER_Read(LEFT);
                Fonc_Orientation(2);
                Already_Requested = true;
            }

            if (getAngleSince(aL,aR) >= 0.95*200)
            {
                if(is_horizontal_detected() || is_vertical_detected())
                {

                    Action = Follow_line_straight;
                    Already_Requested = false;

                }
            }
            break;

    case decelerate_To_0:
        
        if (Already_Requested != true)
        {
        decelerateToStop();
        updateMotorProfiles();
        Already_Requested = true;
        }
    
        if (areProfilesDone())
        {
            Already_Requested = false;
            Left_or_Right(Y_Target);
        }
        

        break;

    case Finished:
        
        decelerateToStop();
        updateMotorProfiles();

        return true;

    default:
        break;
    }
}





bool is_horizontal_detected(){

    Capteurs Valeurs = Binary_Conversion(white);

    if (Next_Step == Retrouver_ligne && (Sens_robot == X_Axis_Neg || Sens_robot == X_Axis_Pos) && (Valeurs.C_Left < -pourcentage_ton_detect || Valeurs.C_Right < -pourcentage_ton_detect || Valeurs.C_Middle < -pourcentage_ton_detect))
    {
        return true;
    }
    
    else if ((Valeurs.C_Left < -pourcentage_ton_detect && Valeurs.C_Right < -pourcentage_ton_detect) || (Valeurs.C_Left < -pourcentage_ton_detect && Valeurs.C_Middle < -pourcentage_ton_detect) || (Valeurs.C_Middle < -pourcentage_ton_detect && Valeurs.C_Right < -pourcentage_ton_detect))
    {
        Action = Horizontal_Detected;
        return true;
    }
    return false;
}

 


bool is_vertical_detected(){

    Capteurs Valeurs = Binary_Conversion(black);

    if (Next_Step == Retrouver_ligne && (Sens_robot == Y_Axis_Neg || Sens_robot == Y_Axis_Pos) && (Valeurs.C_Left > pourcentage_ton_detect || Valeurs.C_Right > pourcentage_ton_detect || Valeurs.C_Middle > pourcentage_ton_detect))
    {
        return true;
    }

    else if (Valeurs.C_Left > pourcentage_ton_detect && Valeurs.C_Right > pourcentage_ton_detect)
    {
        Action = Vertical_Detected;
        return true;
    }
    return false;
}






void Fonc_Orientation(int ajout){

    Sens_robot += ajout;

    if (Sens_robot < 0)
    {
        Sens_robot = 4 + Sens_robot;
    }

    if (Sens_robot > 3)
    {
        Sens_robot = Sens_robot - 4;
    }
}







void Left_or_Right(int Target_Y){

    if (Y_NOW<Target_Y)
    {
        if (Sens_robot == X_Axis_Neg)
        {
            Action = TurnRight;
        }
        else if (Sens_robot == X_Axis_Pos)
        {
            Action = TurnLeft;
        }  
    }
    else if (Y_NOW>Target_Y)
    {
        if (Sens_robot == X_Axis_Neg)
        {
            Action = TurnLeft;
        }
        else if (Sens_robot == X_Axis_Pos)
        {
            Action = TurnRight;
        }  
    }
}



int Wich_Line_Follow(){

if (Sens_robot == X_Axis_Neg || Sens_robot == X_Axis_Pos)
{
    return ton::white;
}
else
{
    return ton::black;
}
}

void fonction_How_much_turn(){

if (X_NOW == X_Target)
        {
            if (Y_NOW < Y_Target)
            {
                if (Sens_robot == Y_Axis_Pos)
                { 
                    Action = Follow_line_straight;
                }
                else if (Sens_robot == X_Axis_Pos)
                {
                    Tourner_Droite();
                }
                else if (Sens_robot == Y_Axis_Neg)
                {
                    Tourner_180();
                }
                else if (Sens_robot == X_Axis_Neg)
                {
                    Tourner_Gauche();
                }
            }
            else if (Y_NOW > Y_Target)
            {
                if (Sens_robot == Y_Axis_Neg)
                {
                    Action = Follow_line_straight;
                }
                else if (Sens_robot == X_Axis_Pos)
                {
                    Tourner_Gauche();
                }
                else if (Sens_robot == Y_Axis_Pos)
                {
                    Tourner_180();
                }
                else if (Sens_robot == X_Axis_Neg)
                {
                    Tourner_Droite(); 
                }
            }
        }
        

        else if (X_NOW < X_Target)
        {
            if (Sens_robot == X_Axis_Pos)
            {
                Action = Follow_line_straight;
            }
            else if (Sens_robot == Y_Axis_Neg)
            {
                Tourner_Droite();
            }
            else if (Sens_robot == Y_Axis_Pos)
            {
                Tourner_Gauche();
            }
            else if (Sens_robot == X_Axis_Neg)
            {
                Tourner_180();
            }
        }

        else if (X_NOW > X_Target)
        {
            if (Sens_robot == X_Axis_Neg)
            {
                Action = Follow_line_straight;
            }
            else if (Sens_robot == Y_Axis_Neg)
            {
               Tourner_Gauche();
            }
            else if (Sens_robot == Y_Axis_Pos)
            {
                Tourner_Droite();
            }
            else if (Sens_robot == X_Axis_Pos)
            {
                Tourner_180();
            }
        }

}


void Tourner_180(){

    Next_Step = U_Turn;
    Action = advance_5;
}

void Tourner_Droite(){

    Next_Step = TurnRight;
    Action = advance_5;
}

void Tourner_Gauche(){

    Next_Step = TurnLeft;
    Action = advance_5;
}