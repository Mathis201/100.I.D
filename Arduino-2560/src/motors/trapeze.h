#pragma once

void initProfile0(float startTime, float startPos, float finalPos,
                  float maxSpeed, float accel, float initialVelocity, float endVelocity);
float getProfiled0PositionAtTime(float currentTime);
bool isProfile0Done(float currentTime);

void initProfile1(float startTime, float startPos, float finalPos, float maxSpeed,
                  float accel, float initialVelocity, float endVelocity);
float getProfiled1PositionAtTime(float currentTime);
bool isProfile1Done(float currentTime);