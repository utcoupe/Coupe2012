#include "command.h"

void setup()
{
	initSerialLink();
	initPinceControl();
	initEncoders();
	initSensor();
	setupTourelle();
}

void loop()
{
	readIncomingData();
	encoderSafe();
	sensorTrigger();
	loopTourelle();
	testAV();
	testAR();
}


