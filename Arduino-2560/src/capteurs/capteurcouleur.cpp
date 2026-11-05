#include "Adafruit_TCS34725.h"
#include "constants.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int tolerence = 95;
float toleranceRatio = 0.04;
const unsigned long delaiEntreLectures = 60;

struct Couleur {
    int red;
    int green;
    int blue;
    int clear;
};

Couleur couleurscapteur[5];

void initcapteurcouleur() {
    tcs.begin();

    couleurscapteur[ActionColors::BLUE] = Couleur{250, 415, 390, 1100};
    couleurscapteur[ActionColors::PINK_] = Couleur{670, 490, 380, 1550};
    couleurscapteur[ActionColors::YELLOW] = Couleur{600, 650, 345, 1600};
    couleurscapteur[ActionColors::GREEN] = Couleur{380, 550, 425, 1450};
    couleurscapteur[ActionColors::ORANGE] = Couleur{550, 480, 280, 1400};
}

bool comparecouleur(Couleur couleurentree, Couleur couleurreference) {
    if (abs(couleurentree.red - couleurreference.red) > tolerence)
        return false;
    if (abs(couleurentree.green - couleurreference.green) > tolerence)
        return false;
    if (abs(couleurentree.blue - couleurreference.blue) > tolerence)
        return false;
    if (abs(couleurentree.clear - couleurreference.clear) > tolerence)
        return false;
    return true;
}

bool compareratios(Couleur couleurentree, Couleur couleurreference) {
    if (couleurentree.clear <= 0 || couleurreference.clear <= 0)
        return false;

    float rE = (float)couleurentree.red / couleurentree.clear;
    float gE = (float)couleurentree.green / couleurentree.clear;
    float bE = (float)couleurentree.blue / couleurentree.clear;

    float rR = (float)couleurreference.red / couleurreference.clear;
    float gR = (float)couleurreference.green / couleurreference.clear;
    float bR = (float)couleurreference.blue / couleurreference.clear;

    if (fabs(rE - rR) > toleranceRatio)
        return false;
    if (fabs(gE - gR) > toleranceRatio)
        return false;
    if (fabs(bE - bR) > toleranceRatio)
        return false;
    return true;
}

Couleur lirecouleurbrute() {
    uint16_t red, green, blue, clear;
    tcs.getRawData(&red, &green, &blue, &clear);
    return Couleur{red, green, blue, clear};
}

ActionColors identifiercouleur(Couleur actuel) {
    for (int i = 0; i < 5; i++) {
        if (comparecouleur(actuel, couleurscapteur[i]) &&
            compareratios(actuel, couleurscapteur[i])) {
            return static_cast<ActionColors>(i);
        }
    }
    return ActionColors::NO_COLOR;
}

enum EtatLecture {
    LECTURE_1,
    ATTENTE_2
};

static EtatLecture etatLecture = LECTURE_1;
static ActionColors couleurCandidate = ActionColors::NO_COLOR;
static ActionColors dernierResultat = ActionColors::NO_COLOR;
static unsigned long tempsPremiereLecture = 0;

ActionColors lirecouleuractuel() {
    switch (etatLecture) {

    case LECTURE_1: {
        tcs.setInterrupt(false);

        ActionColors c = identifiercouleur(lirecouleurbrute());

        if (c == ActionColors::NO_COLOR) {
            dernierResultat = ActionColors::NO_COLOR;
        } else {
            couleurCandidate = c;
            tempsPremiereLecture = millis();
            etatLecture = ATTENTE_2;
        }
        break;
    }

    case ATTENTE_2: {
        if (millis() - tempsPremiereLecture >= delaiEntreLectures) {
            ActionColors c2 = identifiercouleur(lirecouleurbrute());

            dernierResultat = (c2 == couleurCandidate) ? couleurCandidate
                                                       : ActionColors::NO_COLOR;
            etatLecture = LECTURE_1;
        }
        break;
    }
    }

    return dernierResultat;
}