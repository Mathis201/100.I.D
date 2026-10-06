#include "trapeze.h"
#include <LibRobus.h>

constexpr float kTicksPerRotation = 3200.0;
constexpr float kWheelRadiusMm = 38.1;
constexpr float kTrackWidthMm = 187.2;
constexpr float kWheelCircumference = 2.0 * PI * kWheelRadiusMm;
constexpr float fTicksToMm = kWheelCircumference / kTicksPerRotation;

constexpr float maxRecordedSpeed = 915; // mm / s

constexpr float kMaxSpeed = 650; // mm / s
constexpr float kAccel = 1000;   // mm / s ^ 2

constexpr float kP = 0.02;

constexpr float kRefreshDelayMs = 20;
constexpr float kTransitionDelayMs = 100;

float currentLeftPosition = 0;
float currentRightPosition = 0;

float currentLeftVelocity = 0;
float currentRightVelocity = 0;

static void setMotorSpeeds(float leftSpeed, float rightSpeed) {
    MOTOR_SetSpeed(LEFT, leftSpeed);
    MOTOR_SetSpeed(RIGHT, rightSpeed);
}

static float calculateMotorOutput(float motorPosition, float motorTarget) {
    float calculated = (motorTarget - motorPosition) * kP;
    return calculated;
}

void updateMotorInfo(float deltaTimeMs) {
    float newLeftPos = ENCODER_Read(LEFT) * fTicksToMm;
    if (deltaTimeMs != 0) {
        currentLeftVelocity = (newLeftPos - currentLeftPosition) / (deltaTimeMs / 1000);
    }
    currentLeftPosition = newLeftPos;

    float newRightPos = ENCODER_Read(RIGHT) * fTicksToMm;
    if (deltaTimeMs != 0) {
        currentRightVelocity = (newRightPos - currentRightPosition) / (deltaTimeMs / 1000);
    }
    currentRightPosition = newRightPos;
}

void sendMotorsToTargets(float leftTarget, float rightTarget) {
    float startTime = millis();
    float lastTime = startTime;
    float elapsedTime = 0;

    float startLeft = currentLeftPosition;
    float startRight = currentRightPosition;
    initProfile0(leftTarget - startLeft, kMaxSpeed, kAccel, 0, 0);
    initProfile1(rightTarget - startRight, kMaxSpeed, kAccel, 0, 0);

    while (!isProfile0Done(elapsedTime) || !isProfile1Done(elapsedTime)) {
        float currentTime = millis();
        updateMotorInfo(currentTime - lastTime);

        float calculatedLeft = getProfiled0PositionAtTime(elapsedTime);
        float calculatedRight = getProfiled1PositionAtTime(elapsedTime);

        float leftSpeed = calculateMotorOutput(currentLeftPosition, calculatedLeft + startLeft);
        float rightSpeed = calculateMotorOutput(currentRightPosition, calculatedRight + startRight);
        setMotorSpeeds(leftSpeed, rightSpeed);

        lastTime = currentTime;
        delay(kRefreshDelayMs);
        elapsedTime = (millis() - startTime) / 1000;
    };

    delay(kTransitionDelayMs);
}

void moveStraightDistance(float distanceMm) {
    sendMotorsToTargets(currentLeftPosition + distanceMm, currentRightPosition + distanceMm);
}

void turnDegrees(float degrees) {
    float distance = ((degrees * PI / 180) * kTrackWidthMm) / 2;
    sendMotorsToTargets(currentLeftPosition - distance, currentRightPosition + distance);
}

void stopMotors() {
    setMotorSpeeds(0, 0);
}