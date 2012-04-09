#include "command.h"

void setup()
{

  initSerialLink();

  cherche_moteurs();

  pinMode(PIN_SWITCH_HAUT, INPUT);
  pinMode(PIN_SWITCH_BAS, INPUT);
  digitalWrite(PIN_SWITCH_HAUT, HIGH);
  digitalWrite(PIN_SWITCH_BAS, HIGH);

  attachInterrupt(2, fdcHaut, CHANGE); //pin 21
  attachInterrupt(3, fdcBas, CHANGE); //pin 20
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
