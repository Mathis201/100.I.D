
#ifndef PID_follow_line_H_
#define PID_follow_line_H_

struct Capteurs
{
    float C_Left;
    float C_Middle;
    float C_Right;
};

enum Alphabet{A,B,C,D,E,F,G,H,I,J};

enum Actions{Horizontal_Detected, Vertical_Detected, Follow_line_straight, TurnRight, TurnLeft, Put_Pawn};

enum Orientation{X_Axis_Pos, X_Axis_Neg, Y_Axis_Pos, Y_Axis_Neg};

enum ton{white, black};

Capteurs Binary_Conversion();

void initLineSensor();

bool Go_To_Position_XY(int x, int y);

void Follow_line(int ton);




#endif