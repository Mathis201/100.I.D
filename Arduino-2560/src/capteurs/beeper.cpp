#include "beeper.h"
#include "LibRobus.h"

constexpr int kOnDelay = 100;
constexpr int kOffDelay = 100;

bool startedBeep = false;
bool currentlyOn = false;
int countLeft = 0;

int lastSwitchTime = 0;

void requestBeep(int count) {
    if (countLeft != 0) {
        lastSwitchTime = 0;
        startedBeep = false;
    }
    countLeft = count;
}

void updateBeeper() {
    if (countLeft > 0) {
        if (!startedBeep) {
            AX_BuzzerON();
            startedBeep = true;
            currentlyOn = true;
            lastSwitchTime = millis();
        }

        if (startedBeep && currentlyOn && (millis() - lastSwitchTime >= kOnDelay)) {
            AX_BuzzerOFF();
            currentlyOn = false;
            lastSwitchTime = millis();
        }

        if (startedBeep && !currentlyOn && (millis() - lastSwitchTime >= kOffDelay)) {
            startedBeep = false;
            countLeft--;
        }
    }
}
