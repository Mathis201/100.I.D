#pragma once

void initLineSensor();
int getNormalizedLineReading();
bool IsLineDetected();

struct Capteurs;
Capteurs Binary_Conversion();
void Adapt_Curve();