#ifndef __PERCPETEUR_H__
#define __PERCEPTEUR_H__

#include "Arduino.h"

#include "message.h"

#include "AFMotor.h"

#define NB_MOTORS_PERCEPTEUR		1

#define PIN_SWITCH_HAUT			21
#define PIN_SWITCH_BAS			20

class Percepteur {

 public:
  Percepteur();
  int set_pwm(int ordre, int pwm);

 private:
  int pwm;
  int sens;
  AF_DCMotor * motors;

};

void fdcHaut();
void fdcBas();

extern Percepteur g_percepteur;



#endif
