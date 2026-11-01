#pragma once

void initLineSensor();
int getNormalizedLineReading();
bool IsLineDetected(int nbvoulu);

struct Capteurs;
Capteurs Binary_Conversion();
void Adapt_Curve();