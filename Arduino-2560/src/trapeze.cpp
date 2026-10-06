#include "trapeze.h"
#include <Arduino.h>
#include <math.h>
#include <stdlib.h>

static float _maxSpeed0;
static float _accel0;
static float _initialVelocity0;
static float _endVelocity0;

static float _tA0;
static float _tB0;
static float _tF0;

static float _maxSpeed1;
static float _accel1;
static float _initialVelocity1;
static float _endVelocity1;

static float _tA1;
static float _tB1;
static float _tF1;

static void initTrapezoidalProfile0(float finalPos, float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // assign parameters
    _maxSpeed0 = maxSpeed;
    _accel0 = accel;
    _initialVelocity0 = initialVelocity;
    _endVelocity0 = endVelocity;

    // if negative end, need to flip accel and maxSpeed to be negative
    if (finalPos < 0) {
        _maxSpeed0 = -_maxSpeed0;
        _accel0 = -_accel0;
    }

    // calculate timing
    _tF0 = ((2 * _accel0 * finalPos) + (2 * _maxSpeed0 * _maxSpeed0) + (_initialVelocity0 * _initialVelocity0) +
            (_endVelocity0 * _endVelocity0) - (2 * _maxSpeed0 * (_initialVelocity0 + _endVelocity0))) /
           (2 * _accel0 * _maxSpeed0);
    _tA0 = (_maxSpeed0 - _initialVelocity0) / _accel0;
    _tB0 = (_endVelocity0 - _maxSpeed0) / _accel0 + _tF0;
}

static void initTriangularProfile0(float finalPos, float accel, float initialVelocity, float endVelocity) {
    // values are all calculated based on isolated equations for triangular profile
    // max speed is not the one the user passed in, can't reach it because of triangular profile
    float maxSpeed = sqrt(accel * abs(finalPos) + (initialVelocity * initialVelocity + endVelocity * endVelocity) / 2);

    // assign parameters
    _maxSpeed0 = maxSpeed;
    _accel0 = accel;
    _initialVelocity0 = initialVelocity;
    _endVelocity0 = endVelocity;

    // if negative end, need to flip accel and maxSpeed to be negative
    if (finalPos < 0) {
        _maxSpeed0 = -_maxSpeed0;
        _accel0 = -accel;
    }

    // calculate timing
    _tA0 = (_maxSpeed0 - _initialVelocity0) / _accel0;
    _tB0 = _tA0; // no flat part between tA and tB
    _tF0 = (_maxSpeed0 - _endVelocity0) / _accel0 + _tA0;
}

static void initTrapezoidalProfile1(float finalPos, float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // assign parameters
    _maxSpeed1 = maxSpeed;
    _accel1 = accel;
    _initialVelocity1 = initialVelocity;
    _endVelocity1 = endVelocity;

    // if negative end, need to flip accel and maxSpeed to be negative
    if (finalPos < 0) {
        _maxSpeed1 = -_maxSpeed1;
        _accel1 = -_accel1;
    }

    // calculate timing
    _tF1 = ((2 * _accel1 * finalPos) + (2 * _maxSpeed1 * _maxSpeed1) + (_initialVelocity1 * _initialVelocity1) +
            (_endVelocity1 * _endVelocity1) - (2 * _maxSpeed1 * (_initialVelocity1 + _endVelocity1))) /
           (2 * _accel1 * _maxSpeed1);
    _tA1 = (_maxSpeed1 - _initialVelocity1) / _accel1;
    _tB1 = (_endVelocity1 - _maxSpeed1) / _accel1 + _tF1;
}

static void initTriangularProfile1(float finalPos, float accel, float initialVelocity, float endVelocity) {
    // values are all calculated based on isolated equations for triangular profile
    // max speed is not the one the user passed in, can't reach it because of triangular profile
    float maxSpeed = sqrt(accel * abs(finalPos) + (initialVelocity * initialVelocity + endVelocity * endVelocity) / 2);

    // assign parameters
    _maxSpeed1 = maxSpeed;
    _accel1 = accel;
    _initialVelocity1 = initialVelocity;
    _endVelocity1 = endVelocity;

    // if negative end, need to flip accel and maxSpeed to be negative
    if (finalPos < 0) {
        _maxSpeed1 = -_maxSpeed1;
        _accel1 = -accel;
    }

    // calculate timing
    _tA1 = (_maxSpeed1 - _initialVelocity1) / _accel1;
    _tB1 = _tA1; // no flat part between tA and tB
    _tF1 = (_maxSpeed1 - _endVelocity1) / _accel1 + _tA1;
}

void initProfile0(float finalPos, float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // vérifier si peut atteindre vitesse max
    // à cette accel, pour atteindre vitesse max et redescendre, distance nécessaire plus que distance demandée?
    // si oui, impossible de faire trapèze, utiliser triangle
    float minPosNeeded = (2 * maxSpeed * maxSpeed - initialVelocity * initialVelocity - endVelocity * endVelocity) / (2 * accel);
    if (minPosNeeded > abs(finalPos)) {
        initTriangularProfile0(finalPos, accel, initialVelocity, endVelocity);
        Serial.println("triangular");
    } else {
        initTrapezoidalProfile0(finalPos, maxSpeed, accel, initialVelocity, endVelocity);
        Serial.println("trapezoidal");
    }
}

float getProfiled0PositionAtTime(float currentTime) {
    if (currentTime < _tA0) {
        // still in rising part of curve
        return _initialVelocity0 * currentTime + (_accel0 * currentTime * currentTime) / 2;
    } else {
        float risingPos = _initialVelocity0 * _tA0 + (_accel0 * _tA0 * _tA0) / 2;
        // in flat part of curve, before fall
        if (currentTime < _tB0) {
            return risingPos + _maxSpeed0 * (currentTime - _tA0);
        } else {
            // in falling part of curve
            float flatPos = _maxSpeed0 * (_tB0 - _tA0);
            float fallingPos = _maxSpeed0 * (currentTime - _tB0) - (_accel0 * (currentTime - _tB0) * (currentTime - _tB0)) / 2;
            return risingPos + flatPos + fallingPos;
        }
    }
}

bool isProfile0Done(float currentTime) {
    return (currentTime > _tF0);
}

void initProfile1(float finalPos, float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // vérifier si peut atteindre vitesse max
    // à cette accel, pour atteindre vitesse max et redescendre, distance nécessaire plus que distance demandée?
    // si oui, impossible de faire trapèze, utiliser triangle
    float minPosNeeded = (2 * maxSpeed * maxSpeed - initialVelocity * initialVelocity - endVelocity * endVelocity) / (2 * accel);
    if (minPosNeeded > abs(finalPos)) {
        initTriangularProfile1(finalPos, accel, initialVelocity, endVelocity);
    } else {
        initTrapezoidalProfile1(finalPos, maxSpeed, accel, initialVelocity, endVelocity);
    }
}

float getProfiled1PositionAtTime(float currentTime) {
    if (currentTime < _tA1) {
        // still in rising part of curve
        return _initialVelocity1 * currentTime + (_accel1 * currentTime * currentTime) / 2;
    } else {
        float risingPos = _initialVelocity1 * _tA1 + (_accel1 * _tA1 * _tA1) / 2;
        // in flat part of curve, before fall
        if (currentTime < _tB1) {
            return risingPos + _maxSpeed1 * (currentTime - _tA1);
        } else {
            // in falling part of curve
            float flatPos = _maxSpeed1 * (_tB1 - _tA1);
            float fallingPos = _maxSpeed1 * (currentTime - _tB1) - (_accel1 * (currentTime - _tB1) * (currentTime - _tB1)) / 2;
            return risingPos + flatPos + fallingPos;
        }
    }
}

bool isProfile1Done(float currentTime) {
    return (currentTime > _tF1);
}