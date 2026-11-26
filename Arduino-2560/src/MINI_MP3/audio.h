#pragma once
#include <Arduino.h>

enum Son :uint8_t
    {
        Rate = 1,
        Touche = 2,
        Coule = 3
    };

void audio_init(uint8_t volume = 20);

void audio_jouer(Son son); 

void audio_stop();

void audio_volume(uint8_t volume);

// Retourne le nombre de fichiers vus sur la carte SD (-1 = pas de réponse du module)
int16_t audio_nb_pistes();