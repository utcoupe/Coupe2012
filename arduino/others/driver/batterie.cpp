#include "WProgram.h"
#include "wiring.h"

#include "batterie.h"


void loopBatterie()
{
	int value;

	value = analogRead(1);

	Serial.println(value);
}


