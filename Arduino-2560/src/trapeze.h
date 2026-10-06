#pragma once

void initProfile0(float finalPos, float maxSpeed, float accel, float initialVelocity = 0, float endVelocity = 0);
float getProfiled0PositionAtTime(float currentTime);
bool isProfile0Done(float time);

void initProfile1(float finalPos, float maxSpeed, float accel, float initialVelocity = 0, float endVelocity = 0);
float getProfiled1PositionAtTime(float currentTime);
bool isProfile1Done(float time);