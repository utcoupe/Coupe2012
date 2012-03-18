#include "command.h"

void setup()
{
  initSerialLink();

  cherche_moteurs();
}

long long int timeStart;
void loop()
{
  timeStart = micros();
  // La del est allumee pendant le traitement

  readIncomingData();

  // On attend le temps qu'il faut pour boucler
  long udelay = 2000-(micros()-timeStart);
  if(udelay>0)
    delayMicroseconds(udelay);
}
