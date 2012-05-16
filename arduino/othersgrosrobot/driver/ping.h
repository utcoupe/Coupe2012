#ifndef PING_H_
#define PING_H_

#include "include_arduino.h"


#define ID_MSG_US		104
#define PIN_US			22



unsigned int getDistance(unsigned char pingPin);
long microsecondsToInches(long int microseconds);
long microsecondsToCentimeters(long int microseconds);
void usTrigger();

#endif


