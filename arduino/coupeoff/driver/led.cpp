#include "include_arduino.h"




#include "led.h"



void eteindreLed(int i)
{
	digitalWrite(i, LOW);
}

void allumeLed(int i)
{
	digitalWrite(i, HIGH);
}


