#include "WProgram.h"

#include "parameters.h"
#include "robot.h"
#include "encoder.h"
#include "message.h"
#include "fifo.h"
#include "fifo_to_robot.h"

unsigned long index = 0;
unsigned long timeStart = 0;


void setup()
{
	/*Active les pwm*/
	initPWM();
	/*Active les interruptions sur les encodeurs*/
	initEncoders();
	/*Active la liaison serie*/
	initSerialLink();
	
	robot.init();
	
	// LED qui n'en est pas une
	pinMode(16,OUTPUT);
}


void loop(){
	
	/* on note le temps de debut */
	timeStart = micros();

	// La del est allumee pendant le traitement
	digitalWrite(16, HIGH);
	
	/* zone programmation libre */
	
	robot.update(DUREE_CYCLE);
	
	readIncomingData();

	if (robot.goalIsReached() and !fifo.isEmpty())
	{
		FifoObj_to_robot(fifo.pop());
	}
	
	/* fin zone de programmation libre */

	/* On eteint la del */
	digitalWrite(16, LOW);
	
	/* On attend le temps qu'il faut pour boucler */
	long udelay = DUREE_CYCLE*1000-(micros()-timeStart);
	if(udelay<0)
		Serial.println("ouch : mainloop trop longue");
	else
		delayMicroseconds(udelay);
}


