#ifndef PING_H_
#define PING_H_

#include <WProgram.h>

unsigned int getDistance(unsigned char pingPin);
long microsecondsToInches(long int microseconds);
long microsecondsToCentimeters(long int microseconds);
long usTrigger();

#endif


