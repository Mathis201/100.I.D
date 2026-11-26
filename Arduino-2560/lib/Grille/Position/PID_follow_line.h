
#ifndef PID_follow_line_H_
#define PID_follow_line_H_

struct Capteurs
{
    float C_Left;
    float C_Middle;
    float C_Right;
};

enum ton{white, black};

Capteurs Binary_Conversion();

void initLineSensor();

void Follow_line(int ton);

int getNormalizedLineReading();

void Live_Error_Calc(int side);

float motor_calc();

void Deriv_Calc_PID();

void follow_line_calc();

#endif