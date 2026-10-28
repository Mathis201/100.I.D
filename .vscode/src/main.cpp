/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: 
Date: Derniere date de modification
*/

#include <LibRobus.h>
int limite_x_g = -1;
int limite_x_d = 1;
int limite_y = 10;

int pos_x = 0;
int pos_y = 0;
int dernier_cote = -1;
bool fin = 0;

int vertpin = 48;
int rougepin = 49;

void avancerCm(float distanceCm) {
  const float ticks_par_cm = 130;

  const float vitesse = 0.5;
  const float vitesseMin = 0.17;
  const float acceleration = 0.01;
  const float Kp = 0.04;
  const float Ki = 0.002;
  const float sommeMax = 500;

  const float ratioDecel = 0.20;
  float ticksCible = distanceCm * ticks_par_cm;
  float ticksDecel = ticksCible * ratioDecel;

  float ticksParcourus = 0;
  float vitesseActuelle = vitesseMin;
  float erreur_accumule = 0;

  float totalGauche = 0;
  float totalDroite = 0;

  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  while (ticksParcourus < ticksCible) {
    delay(10);
    float tickLeft = ENCODER_ReadReset(LEFT);
    float tickRight = ENCODER_ReadReset(RIGHT);

    ticksParcourus += (tickLeft + tickRight) / 2;
    totalGauche += tickLeft;
    totalDroite += tickRight;

    float erreur = tickLeft - tickRight;
    erreur_accumule += erreur;
    erreur_accumule = constrain(erreur_accumule, -sommeMax, sommeMax);

    float correction = (erreur * Kp) + (erreur_accumule * Ki);

    float ticksRestants = ticksCible - ticksParcourus;

    if (ticksRestants < ticksDecel) {
      float vitesseCible = vitesseMin + (vitesse - vitesseMin) * (ticksRestants / ticksDecel);
      vitesseActuelle = min(vitesseActuelle, vitesseCible);
    } else if (vitesseActuelle < vitesse) {
      vitesseActuelle = min(vitesseActuelle + acceleration, vitesse);
    }

    MOTOR_SetSpeed(RIGHT, vitesseActuelle + correction / 2);
    MOTOR_SetSpeed(LEFT, vitesseActuelle - correction / 2);
  }

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  Serial.println("--- avancerCm ---");
  Serial.print("Distance obtenue (cm): "); Serial.println(ticksParcourus / ticks_par_cm);
  Serial.print("Total ticks gauche: "); Serial.println(totalGauche);
  Serial.print("Total ticks droite: "); Serial.println(totalDroite);
  Serial.print("Ecart gauche-droite (ticks): "); Serial.println(totalGauche - totalDroite);
  Serial.print("Erreur accumulee finale: "); Serial.println(erreur_accumule);
}

void tournerDeg(float angle) {
  const float ticks_par_cm = 133.7;
  const float vitesse = 0.3;
  const float vitesseFin = 0.12;
  const float degresDecel = 15;
  const float largeur_entre_roues = 17.95;
  const float Kp = 0.005;
  const float Ki = 0.0001;
  const float sommeMax = 500;

  const int sens = (angle > 0) ? -1 : 1;

  float tickCible = (fabs(angle) / 360) * PI * largeur_entre_roues * ticks_par_cm;
  float tickDecel = (degresDecel / 360) * PI * largeur_entre_roues * ticks_par_cm;
  float ticksParcourus = 0;
  float erreur_accumule = 0;

  float totalGauche = 0;
  float totalDroite = 0;

  ENCODER_ReadReset(LEFT);
  ENCODER_ReadReset(RIGHT);

  while (ticksParcourus < tickCible) {
    delay(10);
    float tickLeft = ENCODER_ReadReset(LEFT);
    float tickRight = ENCODER_ReadReset(RIGHT);

    ticksParcourus += (fabs(tickLeft) + fabs(tickRight)) / 2;

    totalGauche += fabs(tickLeft);
    totalDroite += fabs(tickRight);

    float erreur = fabs(tickRight) - fabs(tickLeft);
    erreur_accumule += erreur;
    erreur_accumule = constrain(erreur_accumule, -sommeMax, sommeMax);

    float correction = (erreur * Kp) + (erreur_accumule * Ki);

    float ticksRestants = tickCible - ticksParcourus;
    float vitesseCommande = vitesse;
    if (ticksRestants < tickDecel) {
      vitesseCommande = vitesseFin + (vitesse - vitesseFin) * (ticksRestants / tickDecel);
    }

    MOTOR_SetSpeed(RIGHT,  sens * (vitesseCommande - correction / 2));
    MOTOR_SetSpeed(LEFT,  -sens * (vitesseCommande + correction / 2));
  }

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  float angleObtenu = (ticksParcourus / ticks_par_cm) / (PI * largeur_entre_roues) * 360;
  Serial.println("--- tournerDeg ---");
  Serial.print("Angle demande: "); Serial.println(angle);
  Serial.print("Angle obtenu (estime): "); Serial.println(angleObtenu);
  Serial.print("Total ticks gauche: "); Serial.println(totalGauche);
  Serial.print("Total ticks droite: "); Serial.println(totalDroite);
  Serial.print("Erreur accumulee finale: "); Serial.println(erreur_accumule);
}

bool verif_mur(){
  if (digitalRead(vertpin) || digitalRead(rougepin))
    return false;
  else {
    return true;
  }
}

bool verif_limite_x_g(){
  if (pos_x - 1 < limite_x_g){
    return false;
  }
  else{
    return true;
  }
}

bool verif_limite_x_d(){
  if (pos_x + 1 > limite_x_d){
    return false;
  }
  else{
    return true;
  }
}

bool essayerCote(int cote){
  if (cote == -1 && !verif_limite_x_g()) return false;
  if (cote == +1 && !verif_limite_x_d()) return false;

  tournerDeg(90 * cote);        // -90 = gauche, +90 = droite
  delay(100);

  bool libre = !verif_mur();
  if (libre){
    avancerCm(50);
    pos_x += cote;

  }

  tournerDeg(-90 * cote);
  delay(100);
  return libre;
}

void decision(){
  while (!fin) {
    if (!verif_mur()) {
      avancerCm(50);
      pos_y += 1;
    }
    else {
      if (!essayerCote(dernier_cote)) {
        if (essayerCote(-dernier_cote)) {
          dernier_cote = -dernier_cote;
        }
      }
    }

    if ((pos_y >= limite_y - 2) && (!verif_mur())) {
      avancerCm(110);
      tournerDeg(360);
      fin = true;
    }
  }
}

void setup() {
  BoardInit();
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
}

void loop() {
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
  delay(10); // Delais pour décharger le CPU

  if (ROBUS_IsBumper(REAR)){ //code principal
    pos_x = 0;
    pos_y = 0;
    dernier_cote = -1;
    fin = 0;
    decision();
  }

  else if (ROBUS_IsBumper(LEFT)){ //mode test
    tournerDeg(90);
    }
  else if (ROBUS_IsBumper(RIGHT))
    avancerCm(50);
  }