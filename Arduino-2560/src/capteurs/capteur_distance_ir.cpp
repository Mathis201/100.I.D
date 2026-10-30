#include <LibRobus.h>

static const float distancesCm[] = {
    10,15,20,25,30,35,40,45,50,55,60,65,70,75,80
};

static const float valeursIR[] = {
    540.04, 346.40, 272.49, 230.66, 193.23, 170.39, 148.22, 137.53, 128.49, 122.59, 110.34,
    106.74,104.66, 100.4, 97.25
};


float distance_interpolation(int mesure)
{
    if (mesure > valeursIR[0] || mesure < valeursIR[14])
    {
        return -1.0;
    }
    for (int i = 0; i < 14; i++) {
        if (mesure <= valeursIR[i] && mesure >= valeursIR[i + 1]){
        return distancesCm[i] + (mesure - valeursIR[i]) 
        * (distancesCm[i + 1] - distancesCm[i])
        / (valeursIR[i + 1] - valeursIR[i]);
    }
    }
    return -1.0;
}

float detecter_distance(int pin_IR){
    int mesure = ROBUS_ReadIR(pin_IR);
    return distance_interpolation(mesure);
}

