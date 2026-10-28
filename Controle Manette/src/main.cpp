
#include <Arduino.h>
#include <LibRobus.h>
#include <manette.h>
#include <IRremote.hpp>

// Pour utiliser le récepteur infrarouge, vous devez brancher la broche "IR" de J23 sur DIGITAL 37 du Arduino.

  int Bout_Wake_Up = 31;
  int Bout_Start = 27;
  int LedRouge = 16;
  int LedVerte = 15; 
  int Infrarouge = 17;

void setup() {
  // put your setup code here, to run once:
  BoardInit();



  pinMode(Bout_Start, INPUT_PULLUP);
  pinMode(Bout_Wake_Up, INPUT_PULLUP);
  // pinMode(Infrarouge, OUTPUT);
  pinMode(LedRouge, OUTPUT);
  pinMode(LedVerte, OUTPUT);

  IrSender.begin(Infrarouge);
}

void loop() {
if (Bout_Start == false)
{
  IrSender.sendNEC(12, 1, 3);
  digitalWrite(LedVerte, HIGH);
  delay(500);
  digitalWrite(LedVerte, LOW);
}
if (Bout_Wake_Up == false)
{
  IrSender.sendNEC(12, 2, 3);
  for (int i = 0; i < 5; i++)
  {
      digitalWrite(LedVerte, HIGH);
      delay(50);
      digitalWrite(LedVerte, LOW);
      delay(50);
  }
  

}
delay(250);

}
