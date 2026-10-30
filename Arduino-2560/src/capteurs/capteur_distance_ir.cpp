#include <LibRobus.h>

float detecter_distance(int pin_IR){
    //int valeur10cm = 680;
    //int valeur80cm = 65;

    int mesure = ROBUS_ReadIR(pin_IR);

    float distance_cm = 11802.03/(mesure + 43.79) - 15.18;

    return distance_cm;
    //TODO: interpolation avec les valeurs mesurées (les valeurs sont à remesurer)

}


