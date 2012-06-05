#include "ping.h"
#include "message.h"



void usTrigger() {
	static long long timePing = 0;
	if (millis() - timePing > 100) {
		timePing = millis();
		int d = analogRead(PIN_US);
		sendMessage(4242, d);
		d *= 1.65;
		sendMessage(ID_MSG_US, d);
	}
}

