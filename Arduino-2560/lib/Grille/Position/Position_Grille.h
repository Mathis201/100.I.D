
#ifndef Position_Grille_H_
#define Position_Grille_H_


enum Alphabet{A,B,C,D,E,F,G,H,I,J};

enum Actions{How_much_turn, advance_5, motor_update, Horizontal_Detected, Vertical_Detected, Follow_line_straight, TurnRight, TurnLeft, U_Turn, Orienter_Robot, decelerate_To_0, Finished, Retrouver_ligne};

enum Orientation{X_Axis_Pos, Y_Axis_Pos, X_Axis_Neg, Y_Axis_Neg};

void Left_or_Right(int Target_Y);

int Wich_Line_Follow();

void Fonc_Orientation(int ajout);

void Request_Reach_Position_XY(int Target_X, int Target_Y);

bool Update_Reach_Position_XY();

bool is_vertical_detected();

bool is_horizontal_detected();

void fonction_How_much_turn();

void Tourner_Gauche();

void Tourner_180();

void Tourner_Droite();


#endif