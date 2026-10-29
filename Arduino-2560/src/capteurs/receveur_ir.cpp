#include "receveur_ir.h"
#include <LibRobus.h>

constexpr int Adresse_receveur = 12;

int Conversion(char tab[]) {
    int Nombre = 0;
    Nombre = Nombre + 128 * tab[0];
    Nombre = Nombre + 64 * tab[1];
    Nombre = Nombre + 32 * tab[2];
    Nombre = Nombre + 16 * tab[3];
    Nombre = Nombre + 8 * tab[4];
    Nombre = Nombre + 4 * tab[5];
    Nombre = Nombre + 2 * tab[6];
    Nombre = Nombre + 1 * tab[7];

    return Nombre;
}

bool Fonc_Verif(char Droit[], char Inverse[]) {
    bool Inversion_Vrai = true;
    for (int d = 0; d < 8; d++) {
        if (Droit[d] == Inverse[d]) {
            Inversion_Vrai = false;
        }
    }

    return Inversion_Vrai;
}

struct Valeurs {
    int Val_Adresse = -1;
    int Val_Commande = -1;
};

Valeurs Decodage() {

    Valeurs decodees;

    char Adresse_Bin[8];
    char Adresse_Inv_Bin[8];
    char Commande_Bin[8];
    char Commande_Inv_Bin[8];

    int32_t lecture = REMOTE_read();
    if (lecture != 0) {
        for (int i = 0; i < 8; i++) {
            Commande_Inv_Bin[i] = lecture % 2;
            lecture = lecture / 2;
        }
        for (int i = 0; i < 8; i++) {
            Commande_Bin[i] = lecture % 2;
            lecture = lecture / 2;
        }
        if (Fonc_Verif(Commande_Bin, Commande_Inv_Bin) == true) {
            decodees.Val_Commande = Conversion(Commande_Bin);
        }

        for (int i = 0; i < 8; i++) {
            Adresse_Inv_Bin[i] = lecture % 2;
            lecture = lecture / 2;
        }
        for (int i = 0; i < 8; i++) {
            Adresse_Bin[i] = lecture % 2;
            lecture = lecture / 2;
        }
        if (Fonc_Verif(Adresse_Bin, Adresse_Inv_Bin) == true) {
            decodees.Val_Adresse = Conversion(Adresse_Bin);
        }
    }
    return decodees;
}

int getRemoteReadCommand() {
    Valeurs retranscrites = Decodage();
    // Serial.print("Adresse: ");
    // Serial.print(retranscrites.Val_Adresse);
    // Serial.print(", Commande: ");
    // Serial.println(retranscrites.Val_Commande);

    if (retranscrites.Val_Adresse == Adresse_receveur) {
        return retranscrites.Val_Commande;
    }

    return -1;
}

// bool Depart() {
//     Valeurs retranscrites = Decodage();

//     if (retranscrites.Val_Adresse == Adresse_receveur) {

//         if (retranscrites.Val_Commande == 1) // Start
//         {
//             for (int i = 0; i < 3; i++) {
//                 AX_BuzzerON(300);
//                 delay(100);
//                 AX_BuzzerOFF;
//                 delay(233);
//             }

//             return true;

//         } else if (retranscrites.Val_Commande == 2) // Wake up
//         {
//             for (int i = 0; i < 2; i++) {
//                 AX_BuzzerON(300);
//                 delay(100);
//                 AX_BuzzerOFF;
//                 delay(233);
//             }

//             return false;
//         }
//     } else {
//         return false;
//     }
// }

// bool Carre_Bleu() {

//     Valeurs retranscrites = Decodage();

//     if (retranscrites.Val_Adresse == Adresse_receveur) {

//         if (retranscrites.Val_Commande == 1) // Start
//         {

//             for (int i = 0; i < 3; i++) {
//                 AX_BuzzerON(300);
//                 delay(100);
//                 AX_BuzzerOFF();
//                 delay(233);
//             }

//             return false;

//         } else if (retranscrites.Val_Commande == 2) // Wake up
//         {
//             for (int i = 0; i < 2; i++) {
//                 AX_BuzzerON(300);
//                 delay(100);
//                 AX_BuzzerOFF();
//                 delay(233);
//             }

//             return true;
//         }
//     } else {
//         return false;
//     }
// }
