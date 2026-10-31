#include "Arduino.h"
#include "constants.h"

#define BLUE_LED_PIN 45
#define GREEN_LED_PIN 46
#define YELLOW_LED_PIN 47
#define PINK_LED_PIN 49
#define ORANGE_LED_PIN 48

constexpr int onPeriod = 10000; // ms

unsigned long Time_Grid[5]{0, 0, 0, 0, 0};
bool Is_OFF[5] = {true};

void initColorLeds() {
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(PINK_LED_PIN, OUTPUT);
    pinMode(ORANGE_LED_PIN, OUTPUT);

    digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    digitalWrite(PINK_LED_PIN, HIGH);
    digitalWrite(ORANGE_LED_PIN, HIGH);
}

void lightUpLed(ActionColors color) {
    int outputPin;
    switch (color) {
    case ActionColors::BLUE: {
        outputPin = BLUE_LED_PIN;
        break;
    }
    case ActionColors::GREEN: {
        outputPin = GREEN_LED_PIN;
        break;
    }
    case ActionColors::YELLOW: {
        outputPin = YELLOW_LED_PIN;
        break;
    }
    case ActionColors::PINK_: {
        outputPin = PINK_LED_PIN;
        break;
    }
    case ActionColors::ORANGE: {
        outputPin = ORANGE_LED_PIN;
        break;
    }
    default:
        return;
    }

    
    if (Is_OFF[static_cast<int>(color)] == true)
    {
        digitalWrite(outputPin, LOW);
        Time_Grid[static_cast<int>(color)] = millis() + onPeriod;
        Is_OFF[static_cast<int>(color)] = false;
    }
}



void timerLed() {
    if (Time_Grid[static_cast<int>(ActionColors::BLUE)] <= millis()) {
      
        digitalWrite(BLUE_LED_PIN, HIGH);
        Is_OFF[static_cast<int>(ActionColors::BLUE)] = true;
       
    }

    if (Time_Grid[static_cast<int>(ActionColors::YELLOW)] <= millis()) {
      

        digitalWrite(YELLOW_LED_PIN, HIGH);
        Is_OFF[static_cast<int>(ActionColors::YELLOW)] = true;
        
    }

    if (Time_Grid[static_cast<int>(ActionColors::GREEN)] <= millis()) {
     
        digitalWrite(GREEN_LED_PIN, HIGH);
        Is_OFF[static_cast<int>(ActionColors::GREEN)] = true;
       
    }

    if (Time_Grid[static_cast<int>(ActionColors::PINK_)] <= millis()) {
        
        digitalWrite(PINK_LED_PIN, HIGH);
        Is_OFF[static_cast<int>(ActionColors::PINK_)] = true;
        
    }

    if (Time_Grid[static_cast<int>(ActionColors::ORANGE)] <= millis()) {
        
        digitalWrite(ORANGE_LED_PIN, HIGH);
        Is_OFF[static_cast<int>(ActionColors::ORANGE)] = true;
        
    }
}
