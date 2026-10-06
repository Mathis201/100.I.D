#pragma once

void updateMotorInfo(float deltaTimeMs);
void stopMotors();

bool areProfilesDone();
void updateMotorProfiles();
void requestStraightDistance(float distanceMm, float targetEndVelocity = 0.0);
void requestTurnDegrees(float degrees);
