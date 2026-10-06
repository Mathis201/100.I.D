#include "trapeze.h"
#include <LibRobus.h>

constexpr float kTicksPerRotation = 3200.0;
constexpr float kWheelRadiusMm = 38.1;
constexpr float kTrackWidthMm = 190.5;
constexpr float kWheelCircumference = 2.0 * PI * kWheelRadiusMm;
constexpr float fTicksToMm = kWheelCircumference / kTicksPerRotation;

constexpr float maxRecordedSpeed = 915; // mm / s

constexpr float kMaxSpeed = 650; // mm / s
constexpr float kAccel = 700;    // mm / s ^ 2

constexpr float kP = 0.02;
constexpr float kS = 0.0;
constexpr float kProfileRefreshDelayMs = 20;

constexpr float kVelocityRefreshDelayMs = 50;

float currentLeftPosition = 0;
float currentRightPosition = 0;

float lastLeftPosition = 0;
float lastRightPosition = 0;

float targetLeftPosition = 0;
float targetRightPosition = 0;

float currentLeftVelocity = 0;
float currentRightVelocity = 0;

float lastVelocityTimeMs = 0;
float lastProfileTimeMs = 0;

static void setMotorOutputs(float leftOutput, float rightOutput) {
    MOTOR_SetSpeed(LEFT, leftOutput);
    MOTOR_SetSpeed(RIGHT, rightOutput);
}

static float calculateMotorOutput(float motorPosition, float motorTarget) {
    float calculated = (motorTarget - motorPosition) * kP;
    return calculated;
}

static void initMotorProfiles(float leftTarget, float rightTarget, float leftFinalVelocity, float rightFinalVelocity) {
    initProfile0(millis() / 1000.0, currentLeftPosition, leftTarget, kMaxSpeed, kAccel, currentLeftVelocity, leftFinalVelocity);
    initProfile1(millis() / 1000.0, currentRightPosition, rightTarget, kMaxSpeed, kAccel, currentRightVelocity, rightFinalVelocity);

    lastProfileTimeMs = millis();
    targetLeftPosition = currentLeftPosition;
    targetRightPosition = currentRightPosition;
}

void updateMotorInfo(float currentTimeMs) {
    // update motor positions
    currentLeftPosition = ENCODER_Read(LEFT) * fTicksToMm;
    currentRightPosition = ENCODER_Read(RIGHT) * fTicksToMm;

    // if last is 0, first time updating, init values to avoid weird speeds
    if (lastVelocityTimeMs == 0) {
        lastVelocityTimeMs = currentTimeMs;
        lastLeftPosition = currentLeftPosition;
        lastRightPosition = currentRightPosition;
    }

    float deltaTimeMs = currentTimeMs - lastVelocityTimeMs;
    if (deltaTimeMs >= kVelocityRefreshDelayMs) {
        currentLeftVelocity = (currentLeftPosition - lastLeftPosition) / (deltaTimeMs / 1000);
        currentRightVelocity = (currentRightPosition - lastRightPosition) / (deltaTimeMs / 1000);

        lastVelocityTimeMs = currentTimeMs;
        lastLeftPosition = currentLeftPosition;
        lastRightPosition = currentRightPosition;
    }

    // Serial.print("Right: ");
    // Serial.print(currentRightPosition);
    // Serial.print(", Left: ");
    // Serial.println(currentLeftPosition);
}

void stopMotors() {
    setMotorOutputs(0, 0);
}

bool areProfilesDone() {
    return isProfile0Done((millis()) / 1000.0) &&
           isProfile1Done((millis()) / 1000.0);
}

void updateMotorProfiles() {
    if ((millis() - lastProfileTimeMs) >= kProfileRefreshDelayMs) {
        Serial.print("Projected Left: ");
        Serial.print(targetLeftPosition);
        Serial.print(", Actual Left: ");
        Serial.println(currentLeftPosition);

        float leftOutput = calculateMotorOutput(currentLeftPosition, targetLeftPosition);
        float rightOutput = calculateMotorOutput(currentRightPosition, targetRightPosition);

        if (leftOutput > 0) {
            leftOutput += kS;
        } else if (leftOutput < 0) {
            leftOutput -= kS;
        }

        if (rightOutput > 0) {
            rightOutput += kS;
        } else if (rightOutput < 0) {
            rightOutput -= kS;
        }

        setMotorOutputs(leftOutput, rightOutput);

        if (!areProfilesDone()) {
            targetLeftPosition = getProfiled0PositionAtTime(millis() / 1000.0);
            targetRightPosition = getProfiled1PositionAtTime(millis() / 1000.0);
        }

        lastProfileTimeMs = millis();
    }
}

void requestStraightDistance(float distanceMm, float targetEndVelocity) {
    initMotorProfiles(currentLeftPosition + distanceMm, currentRightPosition + distanceMm, targetEndVelocity, targetEndVelocity);
}

void requestTurnDegrees(float degrees) {
    float distanceMm = ((degrees * PI / 180) * kTrackWidthMm) / 2;
    initMotorProfiles(currentLeftPosition - distanceMm, currentRightPosition + distanceMm, 0.0, 0.0);
}