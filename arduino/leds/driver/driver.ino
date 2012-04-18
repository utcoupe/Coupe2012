#include "command.h"

void setup()
{
	initSerialLink();
	for (int i=2; i<11; ++i)
	{
		pinMode(i, OUTPUT);
	}
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


