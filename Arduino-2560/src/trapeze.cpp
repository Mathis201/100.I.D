#include "trapeze.h"
#include <math.h>
#include <stdlib.h>

static float _maxSpeed0;
static float _accel0;
static float _initialVelocity0;
static float _endVelocity0;
static float _startPos0;
static float _startTime0;

static float _tA0;
static float _tB0;
static float _tF0;

static float _maxSpeed1;
static float _accel1;
static float _initialVelocity1;
static float _endVelocity1;
static float _startPos1;
static float _startTime1;

static float _tA1;
static float _tB1;
static float _tF1;

static void initTrapezoidalProfile0(float startTime, float startPos, float finalPos,
                                    float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // assign parameters
    _startTime0 = startTime;
    _maxSpeed0 = maxSpeed;
    _accel0 = accel;
    _initialVelocity0 = initialVelocity;
    _endVelocity0 = endVelocity;
    _startPos0 = startPos;

    float deltaPos = finalPos - startPos;
    // if negative end, need to flip accel and maxSpeed to be negative
    if (deltaPos < 0) {
        _maxSpeed0 = -_maxSpeed0;
        _accel0 = -_accel0;
    }

    // calculate timing
    _tF0 = ((2 * _accel0 * deltaPos) + (2 * _maxSpeed0 * _maxSpeed0) + (_initialVelocity0 * _initialVelocity0) +
            (_endVelocity0 * _endVelocity0) - (2 * _maxSpeed0 * (_initialVelocity0 + _endVelocity0))) /
           (2 * _accel0 * _maxSpeed0);
    _tA0 = (_maxSpeed0 - _initialVelocity0) / _accel0;
    _tB0 = (_endVelocity0 - _maxSpeed0) / _accel0 + _tF0;
}

static void initTriangularProfile0(float startTime, float startPos, float finalPos,
                                   float accel, float initialVelocity, float endVelocity) {
    float deltaPos = finalPos - startPos;
    // values are all calculated based on isolated equations for triangular profile
    // max speed is not the one the user passed in, can't reach it because of triangular profile
    float maxSpeed = sqrt(accel * abs(deltaPos) + (initialVelocity * initialVelocity + endVelocity * endVelocity) / 2);

    // assign parameters
    _startTime0 = startTime;
    _maxSpeed0 = maxSpeed;
    _accel0 = accel;
    _initialVelocity0 = initialVelocity;
    _endVelocity0 = endVelocity;
    _startPos0 = startPos;

    // if negative end, need to flip accel and maxSpeed to be negative
    if (deltaPos < 0) {
        _maxSpeed0 = -_maxSpeed0;
        _accel0 = -accel;
    }

    // calculate timing
    _tA0 = (_maxSpeed0 - _initialVelocity0) / _accel0;
    _tB0 = _tA0; // no flat part between tA and tB
    _tF0 = (_maxSpeed0 - _endVelocity0) / _accel0 + _tA0;
}

static void initTrapezoidalProfile1(float startTime, float startPos, float finalPos,
                                    float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // assign parameters
    _startTime1 = startTime;
    _maxSpeed1 = maxSpeed;
    _accel1 = accel;
    _initialVelocity1 = initialVelocity;
    _endVelocity1 = endVelocity;
    _startPos1 = startPos;

    float deltaPos = finalPos - startPos;
    // if negative end, need to flip accel and maxSpeed to be negative
    if (deltaPos < 0) {
        _maxSpeed1 = -_maxSpeed1;
        _accel1 = -_accel1;
    }

    // calculate timing
    _tF1 = ((2 * _accel1 * deltaPos) + (2 * _maxSpeed1 * _maxSpeed1) + (_initialVelocity1 * _initialVelocity1) +
            (_endVelocity1 * _endVelocity1) - (2 * _maxSpeed1 * (_initialVelocity1 + _endVelocity1))) /
           (2 * _accel1 * _maxSpeed1);
    _tA1 = (_maxSpeed1 - _initialVelocity1) / _accel1;
    _tB1 = (_endVelocity1 - _maxSpeed1) / _accel1 + _tF1;
}

static void initTriangularProfile1(float startTime, float startPos, float finalPos,
                                   float accel, float initialVelocity, float endVelocity) {
    float deltaPos = finalPos - startPos;
    // values are all calculated based on isolated equations for triangular profile
    // max speed is not the one the user passed in, can't reach it because of triangular profile
    float maxSpeed = sqrt(accel * abs(deltaPos) + (initialVelocity * initialVelocity + endVelocity * endVelocity) / 2);

    // assign parameters
    _startTime1 = startTime;
    _maxSpeed1 = maxSpeed;
    _accel1 = accel;
    _initialVelocity1 = initialVelocity;
    _endVelocity1 = endVelocity;
    _startPos1 = startPos;

    // if negative end, need to flip accel and maxSpeed to be negative
    if (deltaPos < 0) {
        _maxSpeed1 = -_maxSpeed1;
        _accel1 = -accel;
    }

    // calculate timing
    _tA1 = (_maxSpeed1 - _initialVelocity1) / _accel1;
    _tB1 = _tA1; // no flat part between tA and tB
    _tF1 = (_maxSpeed1 - _endVelocity1) / _accel1 + _tA1;
}

void initProfile0(float startTime, float startPos, float finalPos,
                  float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // vérifier si peut atteindre vitesse max
    // à cette accel, pour atteindre vitesse max et redescendre, distance nécessaire plus que distance demandée?
    // si oui, impossible de faire trapèze, utiliser triangle
    float minPosNeeded = (2 * maxSpeed * maxSpeed - initialVelocity * initialVelocity - endVelocity * endVelocity) / (2 * accel);
    if (minPosNeeded > abs(finalPos - startPos)) {
        initTriangularProfile0(startTime, startPos, finalPos, accel, initialVelocity, endVelocity);
    } else {
        initTrapezoidalProfile0(startTime, startPos, finalPos, maxSpeed, accel, initialVelocity, endVelocity);
    }
}

float getProfiled0PositionAtTime(float currentTime) {
    float profileTime = currentTime - _startTime0;
    if (profileTime < _tA0) {
        // still in rising part of curve
        return _startPos0 + _initialVelocity0 * profileTime + (_accel0 * profileTime * profileTime) / 2;
    } else {
        float risingPos = _initialVelocity0 * _tA0 + (_accel0 * _tA0 * _tA0) / 2;
        // in flat part of curve, before fall
        if (profileTime < _tB0) {
            return _startPos0 + risingPos + _maxSpeed0 * (profileTime - _tA0);
        } else {
            // in falling part of curve
            float flatPos = _maxSpeed0 * (_tB0 - _tA0);
            float fallingPos = _maxSpeed0 * (profileTime - _tB0) - (_accel0 * (profileTime - _tB0) * (profileTime - _tB0)) / 2;
            return _startPos0 + risingPos + flatPos + fallingPos;
        }
    }
}

bool isProfile0Done(float currentTime) {
    return ((currentTime - _startTime0) > _tF0);
}

void initProfile1(float startTime, float startPos, float finalPos,
                  float maxSpeed, float accel, float initialVelocity, float endVelocity) {
    // vérifier si peut atteindre vitesse max
    // à cette accel, pour atteindre vitesse max et redescendre, distance nécessaire plus que distance demandée?
    // si oui, impossible de faire trapèze, utiliser triangle
    float minPosNeeded = (2 * maxSpeed * maxSpeed - initialVelocity * initialVelocity - endVelocity * endVelocity) / (2 * accel);
    if (minPosNeeded > abs(finalPos - startPos)) {
        initTriangularProfile1(startTime, startPos, finalPos, accel, initialVelocity, endVelocity);
    } else {
        initTrapezoidalProfile1(startTime, startPos, finalPos, maxSpeed, accel, initialVelocity, endVelocity);
    }
}

float getProfiled1PositionAtTime(float currentTime) {
    float profileTime = currentTime - _startTime1;
    if (profileTime < _tA1) {
        // still in rising part of curve
        return _startPos1 + _initialVelocity1 * profileTime + (_accel1 * profileTime * profileTime) / 2;
    } else {
        float risingPos = _initialVelocity1 * _tA1 + (_accel1 * _tA1 * _tA1) / 2;
        // in flat part of curve, before fall
        if (profileTime < _tB1) {
            return _startPos1 + risingPos + _maxSpeed1 * (profileTime - _tA1);
        } else {
            // in falling part of curve
            float flatPos = _maxSpeed1 * (_tB1 - _tA1);
            float fallingPos = _maxSpeed1 * (profileTime - _tB1) - (_accel1 * (profileTime - _tB1) * (profileTime - _tB1)) / 2;
            return _startPos1 + risingPos + flatPos + fallingPos;
        }
    }
}

bool isProfile1Done(float currentTime) {
    return ((currentTime - _startTime1) > _tF1);
}