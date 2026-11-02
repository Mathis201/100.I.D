#pragma once

struct Capteurs
{
    float C_Left;
    float C_Middle;
    float C_Right;

};

void initLineSensor();
int getNormalizedLineReading();
bool IsLineDetected(int nbVoulu);

Capteurs Binary_Conversion();
void Adapt_Curve();