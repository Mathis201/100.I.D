
#ifndef PID_follow_line_H_
#define PID_follow_line_H_

struct Capteurs
{
    float C_Left;
    float C_Middle;
    float C_Right;
};

enum ton{white, black};

Capteurs Binary_Conversion(int ton);

void initLineSensor();

void Follow_line(int ton);

int getNormalizedLineReading();




#endif