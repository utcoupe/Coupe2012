#ifndef __PERCPETEUR_H__
#define __PERCEPTEUR_H__

#include "WProgram.h"
#include "message.h"

#include "AFMotor.h"

#define NB_MOTORS_MACHIN		1



class Percepteur {

 public:
  Percepteur();
  int set_pwm(int pwm);

 private:
  int pwm;
  int sens;
  AF_DCMotor * motors;

};

void fdcHaut();
void fdcBas();

extern Percepteur g_percepteur;



#endif
