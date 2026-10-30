#include "Adafruit_TCS34725.h"
#include "constants.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int tolerence = 75;

struct Couleur {
    int red;
    int green;
    int blue;
    int clear;
};

Couleur couleurscapteur[5];

void initcapteurcouleur() {
    tcs.begin();

    couleurscapteur[ActionColors::BLUE] = Couleur{289, 480, 444, 1243};
    couleurscapteur[ActionColors::PINK_] = Couleur{762, 542, 442, 1782};
    couleurscapteur[ActionColors::YELLOW] = Couleur{708, 757, 394, 1913};
    couleurscapteur[ActionColors::GREEN] = Couleur{464, 637, 482, 1623};
    couleurscapteur[ActionColors::ORANGE] = Couleur{628, 508, 300, 1479};
}

bool comparecouleur(Couleur couleurentree, Couleur couleurreference) {

    if (!(couleurentree.red <= couleurreference.red + tolerence)) {
        return false;
    }
    if (!(couleurentree.red >= couleurreference.red - tolerence)) {
        return false;
    }
    if (!(couleurentree.green <= couleurreference.green + tolerence)) {
        return false;
    }
    if (!(couleurentree.green >= couleurreference.green - tolerence)) {
        return false;
    }
    if (!(couleurentree.blue <= couleurreference.blue + tolerence)) {
        return false;
    }
    if (!(couleurentree.blue >= couleurreference.blue - tolerence)) {
        return false;
    }
    if (!(couleurentree.clear <= couleurreference.clear + tolerence)) {
        return false;
    }
    if (!(couleurentree.clear >= couleurreference.clear - tolerence)) {
        return false;
    }
    return true;
}

ActionColors lirecouleuractuel() {
    uint16_t red, green, blue, clear;

    tcs.setInterrupt(false); // turn on LED

    // tcs.getRawData(&red, &green, &blue, &clear);
    Couleur actuel{red, green, blue, clear};

    // Serial.print("R: ");
    // Serial.print(red);
    // Serial.print(" G: ");
    // Serial.print(green);
    // Serial.print(" B: ");
    // Serial.print(blue);
    // Serial.print(" Clear: ");
    // Serial.println(clear);

    for (int i = 0; i < 5; i++) {
        if (comparecouleur(actuel, couleurscapteur[i])) {
            return static_cast<ActionColors>(i);
        }
    }
    return ActionColors::NO_COLOR;
}
