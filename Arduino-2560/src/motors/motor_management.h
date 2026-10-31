#pragma once

constexpr float kMaxSpeed = 600; // mm / s
constexpr float kAccel = 600;    // mm / s ^ 2

void updateMotorInfo(float deltaTimeMs);
void stopMotors();

bool areProfilesDone();
void updateMotorProfiles();

void requestStraightDistance(float distanceMm, float targetEndVelocity = 0.0, float maxSpeed = kMaxSpeed, float accel = kAccel);
void requestTurnDegrees(float degrees, float maxSpeed = kMaxSpeed, float accel = kAccel);
void decelerateToStop();

float getAngleSince(long initialLeftTicks, long initialRightTicks);
