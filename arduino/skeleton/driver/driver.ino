#include "command.h"


// initialisations
void setup()
{
	// initialisationdu port série
	initSerialLink();

	// initialisation des pins
	for (int i=2; i<14; ++i)
	{
		pinMode(i, OUTPUT);
	}
}

long long int timeStart;
// fonction appellée à chaque tour de boucle
void loop()
{
	timeStart = micros();

	// lecture des données arrivant
	readIncomingData();

	


	
	// On attend le temps qu'il faut pour boucler
	long udelay = 2000-(micros()-timeStart);
	if(udelay>0)
		delayMicroseconds(udelay);
	
}


