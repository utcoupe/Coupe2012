#include "command.h"
#include "batterie.h"

void setup()
{
	initSerialLink();
}

long long int timeStart;
void loop()
{
	timeStart = micros();
	// La del est allumee pendant le traitement
	digitalWrite(16, HIGH);


	
	readIncomingData();

	loopBatterie();
	

	// On eteint la del
	digitalWrite(16, LOW);

	
	// On attend le temps qu'il faut pour boucler
	long udelay = 2000-(micros()-timeStart);
	if(udelay>0)
		delayMicroseconds(udelay);
	
}


