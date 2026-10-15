#pragma once

#define B_RETURN_CORRECTION 4

void updateMotorInfo(float deltaTimeMs);
void stopMotors();

bool areProfilesDone();
void updateMotorProfiles();

// Init a trapezoidal profile from currentOrientation to requestedOrientation. (LibRobus defined orientations, based on defines)
void requestOrientation(int currentOrientation, int requestedOrientation);

// Init a trapezoidal profile from one grid coordinate to another.
// The robot has to be correctly orientated to allow the movement (facing FRONT or REAR).
void requestXPosition(int currentGridX, int requestedGridX, int currentOrientation);

// Init a trapezoidal profile from one grid coordinate to another.
// The robot has to be correctly orientated to allow the movement (facing LEFT or RIGHT).
void requestYPosition(int currentGridY, int requestedGridY, int currentOrientation);

void requestStraightDistance(float distanceMm, float targetEndVelocity = 0.0);

void requestTurnDegrees(float degrees);