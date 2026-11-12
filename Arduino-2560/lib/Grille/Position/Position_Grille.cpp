
#include "Position_Grille.h"
#include <LibRobus.h>
#include <Arduino.h>
#include "PID_follow_line.h"



int X_NOW = 0;
int Y_NOW = 0;
int X_PAST = 0;
int Y_PAST = 0;
int Sens_robot = X_Axis_Pos;
int Action = Follow_line_straight;



void Go_To_Position_XY(int Target_X, int Target_Y){

    switch (Action)
    {
    case Follow_line_straight:
    
            if (is_horizontal_detected())
            {
                Action = Horizontal_Detected;
                break;
            }
            else if (is_vertical_detected())
            {
                Action = Vertical_Detected;
                break;
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
            }
            else
            {
                Action = Follow_line_straight;
            }
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
                Left_or_Right(Target_X, Target_Y);
            }

            Action = Follow_line_straight;
            break;

    case TurnLeft:
        
            Fonc_Orientation(-1);
            Action = Follow_line_straight;
            break;

    case TurnRight:
        
            Fonc_Orientation(1);
            Action = Follow_line_straight;
            break;

    case Put_Pawn:
        
        break;

    default:
        break;
    }
}





bool is_horizontal_detected(){

    Capteurs Valeurs = Binary_Conversion(white);

    if (Valeurs.C_Left < -.9 && Valeurs.C_Right < -.9)
    {
        Action = Horizontal_Detected;
        return true;
    }
    return false;
}




bool is_vertical_detected(){

    Capteurs Valeurs = Binary_Conversion(black);

    if (Valeurs.C_Left > 0.9 && Valeurs.C_Right > 0.9)
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
        Sens_robot = 4-Sens_robot;
    }
    if (Sens_robot > 3)
    {
        Sens_robot = Sens_robot - 4;
    }
}







void Left_or_Right(int Target_X, int Target_Y){

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

