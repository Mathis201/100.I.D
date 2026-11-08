
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

Capteurs Binary_Conversion();

void initLineSensor();

bool Go_To_Position_XY(int x, int y);

void Follow_line();




#endif