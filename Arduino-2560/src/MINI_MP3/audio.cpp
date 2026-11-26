#include "audio.h"
#include <DIYables_MiniMp3.h>

// Module TX  -> broche 19 (RX1) de l'Arduino
// Module RX  -> broche 18 (TX1) de l'Arduino, via résistance 1 kΩ

#define MP3_SERIAL Serial1 

static DIYables_MiniMp3 mp3;

void audio_volume(uint8_t volume)
    {
        if (volume > 30) 
            volume = 30;
        mp3.setVolume(volume);
    }


void audio_init(uint8_t volume)
    {
        MP3_SERIAL.begin(9600);
        // debug = true : affiche les octets reçus du module dans le moniteur série
        // timeout = 500 ms au lieu de 100 ms (certains modules répondent lentement)
        mp3.begin(MP3_SERIAL, true, 500);
        delay(2000); // laisser le temps au module de lire la carte SD
        audio_volume(volume);
    }

int16_t audio_nb_pistes()
    {
        return mp3.getTrackCount();
    }

void audio_jouer(Son son)
    {
        mp3.play(son);
    }

void audio_stop()
    {
        mp3.stop();
    }


