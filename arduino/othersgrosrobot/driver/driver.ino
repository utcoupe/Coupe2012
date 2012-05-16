#include "command.h"
#include "ping.h"
#include "sensor.h"

long long int timeVerif;
char stateVerif;

void setup()
{

  initSerialLink();

  cherche_moteurs();

	initSensor();
}


void loop()
{
  long long int timeStart = micros();

  for (char i=0; i<NB_MOTEURS; i++) {
    if (ordre[i] != -1) {
      int pos = lire(0, PRESENT_POSITION, i);
      int err = pos - goal[i];
      if(err <= 5 && err >= -5) {
        sendMessage(ordre[i], "ok");
        ordre[i] = -1;
      }
    }
  }

  usTrigger();
  sensorTrigger();

  readIncomingData();

  // On attend le temps qu'il faut pour boucler
  long udelay = 2000-(micros()-timeStart);
  if(udelay>0)
    delayMicroseconds(udelay);
}
