#include "ping.h"
#include "message.h"



unsigned int getDistance(unsigned char pingPin){
	// establish variables for duration of the ping, 
	// and the distance result in inches and centimeters:
	long duration;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(pingPin, OUTPUT);
	digitalWrite(pingPin, LOW);
	delayMicroseconds(2);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pingPin, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH
	// pulse whose duration is the time (in microseconds) from the sending
	// of the ping to the reception of its echo off of an object.
	pinMode(pingPin, INPUT);
	duration = pulseIn(pingPin, HIGH);
	return duration;
}

long microsecondsToInches(long int microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long int microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 58;
}

void usTrigger() {
	static long long timePing = 0;
	if (millis() - timePing > 100) {
		timePing = millis();
		int d = 0;

		d = microsecondsToCentimeters(getDistance(PIN_US));
		sendMessage(ID_MSG_US,d);
	}
}

