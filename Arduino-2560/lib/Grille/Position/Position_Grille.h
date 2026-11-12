
#ifndef Position_Grille_H_
#define Position_Grille_H_


enum Alphabet{A,B,C,D,E,F,G,H,I,J};

enum Actions{Horizontal_Detected, Vertical_Detected, Follow_line_straight, TurnRight, TurnLeft, Put_Pawn, Orienter_Robot};

enum Orientation{X_Axis_Pos, Y_Axis_Neg, X_Axis_Neg, Y_Axis_Pos};

void Left_or_Right(int Target_Y);

int Wich_Line_Follow();

void Fonc_Orientation(int ajout);

void Go_To_Position_XY(int x, int y);

bool is_vertical_detected();

bool is_horizontal_detected();

#endif