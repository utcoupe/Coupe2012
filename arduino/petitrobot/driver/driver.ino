#include "command.h"

void setup()
{

  initSerialLink();

  cherche_moteurs();

  g_percepteur_droit = new Percepteur(0, DROIT_HAUT, DROIT_BAS);
  g_percepteur_gauche = new Percepteur(1, GAUCHE_HAUT, GAUCHE_BAS);

  attachInterrupt(2, fdcDroitHaut, CHANGE); //pin 21
  attachInterrupt(3, fdcDroitBas, CHANGE); //pin 20
  attachInterrupt(0, fdcGaucheHaut, CHANGE); //pin 2
  attachInterrupt(1, fdcGaucheBas, CHANGE); //pin 3
}

long long int timeStart;
void loop()
{
  timeStart = micros();

  readIncomingData();

  // On attend le temps qu'il faut pour boucler
  long udelay = 2000-(micros()-timeStart);
  if(udelay>0)
    delayMicroseconds(udelay);
}
