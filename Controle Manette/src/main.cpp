
#include <Arduino.h>
#include <IRremote.hpp>

int Bout_Start = 22;
int Bout_Wake_Up = 24;
int LedVerte = 26;
int Infrarouge = 28;

void setup() {
    pinMode(Bout_Start, INPUT);
    pinMode(Bout_Wake_Up, INPUT);

    pinMode(LedVerte, OUTPUT);
    pinMode(Infrarouge, OUTPUT);

    IrSender.begin(Infrarouge);

    digitalWrite(LedVerte, HIGH);

    Serial.begin(9600);
}

void loop() {
    Serial.println(digitalRead(Bout_Start));

    if (digitalRead(Bout_Start) == false) {
        digitalWrite(LedVerte, LOW);
        IrSender.sendNEC(12, 1, 3);
        digitalWrite(LedVerte, HIGH);
    }

    if (digitalRead(Bout_Wake_Up) == false) {
        digitalWrite(LedVerte, LOW);
        IrSender.sendNEC(12, 2, 3);
        digitalWrite(LedVerte, HIGH);
    }

    delay(10);
}
