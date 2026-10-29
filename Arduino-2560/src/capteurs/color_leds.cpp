#include "Arduino.h"
#include "constants.h"

#define BLUE_LED_PIN 45
#define GREEN_LED_PIN 46
#define YELLOW_LED_PIN 47
#define PINK_LED_PIN 49
#define ORANGE_LED_PIN 48

void initColorLeds() {
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(PINK_LED_PIN, OUTPUT);
    pinMode(ORANGE_LED_PIN, OUTPUT);
}

void lightUpLed(ActionColors color) {
    // int outputPin;
    // switch (color) {
    // case ActionColors::BLUE: {
    //     outputPin = BLUE_LED_PIN;
    //     break;
    // }
    // case ActionColors::GREEN: {
    //     outputPin = GREEN_LED_PIN;
    //     break;
    // }
    // case ActionColors::YELLOW: {
    //     outputPin = YELLOW_LED_PIN;
    //     break;
    // }
    // case ActionColors::PINK_: {
    //     outputPin = PINK_LED_PIN;
    //     break;
    // }
    // case ActionColors::ORANGE: {
    //     outputPin = ORANGE_LED_PIN;
    //     break;
    // }
    // default:
    //     return;
    // }

    digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(PINK_LED_PIN, HIGH);
    digitalWrite(ORANGE_LED_PIN, HIGH);
}