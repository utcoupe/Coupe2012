#include "command.h"
#include "ping.h"


// initialisations
void setup()
{
	// initialisationdu port série
	initSerialLink();
	cherche_moteurs();
}

long long int timeStart;
// fonction appellée à chaque tour de boucle
void loop()
{
	timeStart = micros();

	// ultra sons
	usTrigger();
	
	// lecture des données arrivant
	readIncomingData();

	


	
	// On attend le temps qu'il faut pour boucler
	long udelay = 2000-(micros()-timeStart);
	if(udelay>0)
		delayMicroseconds(udelay);
	
}


