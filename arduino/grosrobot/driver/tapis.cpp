#include "tapis.h"

#include "WProgram.h"

Tapis g_tapis;

Tapis::Tapis() {
	pinMode(PIN_TAPIS, OUTPUT);
}


int Tapis::actionner(int on) {
	if (on)
		digitalWrite(PIN_TAPIS, HIGH);
	else
		digitalWrite(PIN_TAPIS, LOW);
	
	return 1;
}



