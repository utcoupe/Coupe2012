#ifndef __PERCPETEUR_H__
#define __PERCEPTEUR_H__

#include "Arduino.h"

#include "message.h"

#include "AFMotor.h"

#define NB_MOTORS_PERCEPTEUR		1

#define DROIT_HAUT	21
#define DROIT_BAS		20
#define GAUCHE_HAUT	2
#define GAUCHE_BAS	3

class Percepteur {

 public:
  Percepteur(char i, char pin_haut_, char pin_bas_);
  int set_pwm(int ordre, int pwm);
  void change_haut(char etat);
  void change_bas(char etat);

 private:
  char id;
  int pwm;
  int sens;

  /* etats switchs
     0 : bas
     1 : bas hésitation
     2 : haut
     3 : haut hésitation
     4 : au milieu
  */
  char etat_switchs;

  /* etats moteur
     0 : arrêté
     1 : elevator going up!
     2 : backward
  */
  char etat_moteur;
  int id_ordre;
  char pin_haut;
  char pin_bas;
  AF_DCMotor *moteur;

};

void fdcDroitHaut();
void fdcDroitBas();
void fdcGaucheHaut();
void fdcGaucheBas();

extern Percepteur *g_percepteur_droit;
extern Percepteur *g_percepteur_gauche;

#endif
