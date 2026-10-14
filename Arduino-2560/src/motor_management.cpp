#include "motor_management.h"
#include "trapeze.h"
#include <LibRobus.h>

#define B

constexpr float kTicksPerRotation = 3200.0;
constexpr float kWheelRadiusMm = 38.1;
#ifdef A
constexpr float kTrackWidthMm = 186.9;
#else
constexpr float kTrackWidthMm = 186.9;
#endif
constexpr float kWheelCircumference = 2.0 * PI * kWheelRadiusMm;
constexpr float fTicksToMm = kWheelCircumference / kTicksPerRotation;
constexpr float distancePerCoordinate = 500; // mm

constexpr float maxRecordedSpeed = 915; // mm / s

constexpr float kMaxSpeed = 600; // mm / s
constexpr float kAccel = 550;    // mm / s ^ 2

constexpr float kP = 0.014;
constexpr float kS = 0.025;
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
    initProfile0(millis() / 1000.0, currentLeftPosition, leftTarget, kMaxSpeed, kAccel, 0.0, leftFinalVelocity);
    initProfile1(millis() / 1000.0, currentRightPosition, rightTarget, kMaxSpeed, kAccel, 0.0, rightFinalVelocity);

    lastProfileTimeMs = millis();
    targetLeftPosition = currentLeftPosition;
    targetRightPosition = currentRightPosition;
}

static int orientationToDegrees(int orientation) {
    switch (orientation) {
    case FRONT:
        return 0;
    case LEFT:
        return 90;
    case RIGHT:
        return -90;
    case REAR:
        return 180;
    default:
        return 0;
    }
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
        } else {
            Serial.print("Projected Left: ");
            Serial.print(targetLeftPosition);
            Serial.print(", Actual Left: ");
            Serial.println(currentLeftPosition);

            Serial.print("Projected Right: ");
            Serial.print(targetRightPosition);
            Serial.print(", Actual Right: ");
            Serial.println(currentRightPosition);
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

void requestOrientation(int currentOrientation, int requestedOrientation) {
    int currentDegrees = orientationToDegrees(currentOrientation);
    int requestedDegrees = orientationToDegrees(requestedOrientation);

    int delta = requestedDegrees - currentDegrees;
    // wrap around if too big (REAR is 180, RIGHT is -90, can be simplified)
    if (delta > 180) {
        delta = 180 - delta;
    } else if (delta < -180) {
        delta = 360 + delta;
    }

    requestTurnDegrees(delta);
}

void requestXPosition(int currentGridX, int requestedGridX, int currentOrientation) {
    // guard against wrong orientations
    if (!(currentOrientation == FRONT || currentOrientation == REAR)) {
        return;
    }
    int deltaXMm = (requestedGridX - currentGridX) * distancePerCoordinate;

    // if facing backwards (towards negative x), allow it to go in reverse
    if (currentOrientation == REAR) {
        deltaXMm = -deltaXMm;
    }

    requestStraightDistance(deltaXMm);
}

void requestYPosition(int currentGridY, int requestedGridY, int currentOrientation) {
    // guard against wrong orientations
    if (!(currentOrientation == LEFT || currentOrientation == RIGHT)) {
        return;
    }
    int deltaYMm = (requestedGridY - currentGridY) * distancePerCoordinate;

    // if facing right (towards negative y), allow it to go in reverse
    if (currentOrientation == RIGHT) {
        deltaYMm = -deltaYMm;
    }

    requestStraightDistance(deltaYMm);
}