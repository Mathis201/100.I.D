#include <Arduino.h>
#include <IRremote.hpp>
#include <LibRobus.h>

const int Adresse_receveur = 12;

void setup() {
    // put your setup code here, to run once:
    BoardInit();

    IrReceiver.begin(6);
}

bool Départ() {
    if (IrReceiver.decode() == true) {

        if (IrReceiver.decodedIRData.address == Adresse_receveur) {

            if (IrReceiver.decodedIRData.command == 1) // Start
            {

                for (int i = 0; i < 3; i++) {
                }
                IrReceiver.resume();
                return true;
            } else if (IrReceiver.decodedIRData.command == 2) // Wake up
            {
                for (int i = 0; i < 2; i++) {
                    /* code */
                }
                IrReceiver.resume();
                return false;
            }
        }
        delay(50);
    }
}

bool Carre_Bleu() {
    if (IrReceiver.decode() == true) {

        if (IrReceiver.decodedIRData.address == Adresse_receveur) {

            if (IrReceiver.decodedIRData.command == 1) // Start
            {

                for (int i = 0; i < 3; i++) {
                }
                IrReceiver.resume();
                return false;
            } else if (IrReceiver.decodedIRData.command == 2) // Wake up
            {
                for (int i = 0; i < 2; i++) {
                    /* code */
                }
                IrReceiver.resume();
                return true;
            }
        }
        delay(50);
    }
}