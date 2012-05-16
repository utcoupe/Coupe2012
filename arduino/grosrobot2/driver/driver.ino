#include "command.h"

long long int timeVerif;
char stateVerif;

void setup()
{

  initSerialLink();

  cherche_moteurs();

  pinMode(21, INPUT);
  digitalWrite(21, HIGH);
  attachInterrupt(2, jack_interrupt, CHANGE);
  timeVerif = 0;
}

void jack_interrupt()
{
  if (timeVerif == 0) {
    stateVerif = digitalRead(21);
    timeVerif = millis() + 20;
  }
}

long long int timeStart;
void loop()
{
  timeStart = micros();

	if (timeVerif != 0 && millis() >= timeVerif && digitalRead(21) == stateVerif) {
    timeVerif = 0;
    sendMessage(103, !stateVerif);
  }

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

  readIncomingData();

  // On attend le temps qu'il faut pour boucler
  long udelay = 2000-(micros()-timeStart);
  if(udelay>0)
    delayMicroseconds(udelay);
}
