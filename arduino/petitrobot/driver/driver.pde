#include "command.h"

void setup()
{
  initSerialLink();

  cherche_moteurs();

  attachInterrupt(4, fdcHaut, CHANGE); //pin 18
  attachInterrupt(5, fdcBas, CHANGE); //pin 19
  //ça occupe les pins i²c
  //si on en a besoin faudra mettre les interrupts ailleurs
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
