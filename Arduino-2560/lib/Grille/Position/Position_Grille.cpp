
#include "Position_Grille.h"
#include <LibRobus.h>
#include <Arduino.h>
#include "PID_follow_line.h"
#include <../../src/motors/motor_management.h>
#include <../../src/motors/trapeze.h>



int X_NOW = 0;
int Y_NOW = 0;
int X_PAST = 0;
int Y_PAST = 0;
int Sens_robot = X_Axis_Pos;
int Action = Follow_line_straight;
unsigned long Last_time_line_detected = 0;
float pourcentage_ton_detect = .9;


void Go_To_Position_XY(int Target_X, int Target_Y){

    switch (Action)
    {
    case Follow_line_straight:

            if (millis()-Last_time_line_detected > 750)
            {
                Last_time_line_detected = millis();

                if (is_horizontal_detected() && Wich_Line_Follow() == black)
                {
                    Action = Horizontal_Detected;
                    break;
                }
                else if (is_vertical_detected() && Wich_Line_Follow() == white)
                {
                    Action = Vertical_Detected;
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
            
            if (Y_NOW==Target_Y)
            {
                Action = Put_Pawn;
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
            
            if (X_NOW == Target_X && Y_NOW == Target_Y)
            {
                Action = Put_Pawn;
                break;
            }
            
            else if (X_NOW == Target_X)
            {
                Left_or_Right(Target_Y);
                break;
            }

            
            Action = Follow_line_straight;
            break;
        

    case TurnLeft:
            decelerateToStop();
            updateMotorProfiles();
            requestTurnDegrees(90);
            updateMotorProfiles();
            Fonc_Orientation(-1);
            Action = Follow_line_straight;
            break;

    case TurnRight:
            decelerateToStop();
            updateMotorProfiles();
            requestTurnDegrees(-90);
            updateMotorProfiles();
            Fonc_Orientation(1);
            Action = Follow_line_straight;
            break;

    case Put_Pawn:
        
        decelerateToStop();

        break;

    default:
        break;
    }
}





bool is_horizontal_detected(){

    Capteurs Valeurs = Binary_Conversion(white);

    if (Valeurs.C_Left < -pourcentage_ton_detect && Valeurs.C_Right < -pourcentage_ton_detect || Valeurs.C_Left < -pourcentage_ton_detect && Valeurs.C_Middle < -pourcentage_ton_detect || Valeurs.C_Middle < -pourcentage_ton_detect && Valeurs.C_Right < -pourcentage_ton_detect)
    {
        Action = Horizontal_Detected;
        return true;
    }
    return false;
}

 


bool is_vertical_detected(){

    Capteurs Valeurs = Binary_Conversion(black);

    if (Valeurs.C_Left > pourcentage_ton_detect && Valeurs.C_Right > pourcentage_ton_detect)
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
        if (Sens_robot == X_Axis_Pos)
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
        if (Sens_robot == X_Axis_Pos)
        {
            Action = TurnRight;
        }  
    }
}



int Wich_Line_Follow(){

if (Sens_robot == X_Axis_Neg || Sens_robot == X_Axis_Pos)
{
    return white;
}
else if (Sens_robot == Y_Axis_Neg || Sens_robot == Y_Axis_Pos)
{
    return black;
}
}

