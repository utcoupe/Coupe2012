#include "Arduino.h"


#include "message.h"
#include "encoder.h"
#include "observer.h"
#include "motor.h"
#include "regulator.h"
#include "command.h"

#define VP			1000
#define VI			0
#define VD			100
#define PP			0.01
#define PI			0
#define PD			0
#define SPEED_LIMIT	(double)700 * (double)ENC_MM_TO_TICKS * (double)DUREE_CYCLE / (double)1000
#define ACCEL_LIMIT	1

void setup()
{
	initPWM();
	initEncoders();
	initSerialLink();
	
	// LED qui n'en est pas une
	pinMode(16,OUTPUT);

	regulatorD.setMod(PosAndSpeedRegulator::BOTH)
		.setSpeedPID(VP,VI,VD)
		.setPosPID(PP,PI,PD)
		.setSpeedLimit(SPEED_LIMIT)
		.setAccelLimit(ACCEL_LIMIT)
		.setOutLimit(255);
	regulatorG.setMod(PosAndSpeedRegulator::BOTH)
		.setSpeedPID(VP,VI,VD)
		.setPosPID(PP,PI,PD)
		.setSpeedLimit(SPEED_LIMIT)
		.setAccelLimit(ACCEL_LIMIT)
		.setOutLimit(255);
}

int32_t last = 0;
void loop(){
	/* on note le temps de debut */
	int32_t time_start = micros();

	// La del est allumee pendant le traitement
	digitalWrite(16, HIGH);
	
	/* zone programmation libre */
	
	observerD.compute(G_value_right_enc);
	int pwmD = (int) regulatorD.compute(G_consign, observerD.getX(), observerD.getV());
	setRightPWM(pwmD);

	
	/*observerG.compute(G_value_left_enc);
	int pwmG = (int) regulatorG.compute(G_consign, observerG.getX(), observerG.getV());
	
	setLeftPWM(pwmG);*/
	
	if (G_debug_on and ((time_start - last) > 100000)) {
		/*Serial.print((long int)G_value_left_enc); Serial.print(" "); Serial.println((long int)G_value_right_enc);
		Serial.println(regulator.getSpeedConsign());
		Serial.print(G_consign); Serial.print(" ");
		Serial.print(observerD.getX()); Serial.print(" ");
		Serial.println(observerD.getV());*/
		Serial.print("plot ");
		Serial.print(millis()); Serial.print(" ");
		Serial.print(observerD.mm_getX()); Serial.print(" ");
		Serial.print(observerD.mm_s_getV()); Serial.print(" ");
		Serial.println(pwmD);
		
		last = time_start;
	}
	
	
	readIncomingData();
	
	/* fin zone de programmation libre */

	/* On eteint la del */
	digitalWrite(16, LOW);
	
	/* On attend le temps qu'il faut pour boucler */
	int32_t udelay = DUREE_CYCLE*1000L-(micros()-time_start);
	if(udelay<0)
		Serial.println("ouch : mainloop trop longue");
	else if (udelay > 1000) {
		delay(udelay/1000);
	}
	else {
		delayMicroseconds(udelay);
	}
}


