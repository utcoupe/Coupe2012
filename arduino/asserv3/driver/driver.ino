#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "wiring.h"
#	include "WProgram.h"
#endif


#include "message.h"
#include "command.h"
#include "tools.h"
#include "motor.h"
#include "globals.h"
#include "goal.h"
#include "control.h"


/*
void attachInterrupts() {
	attachInterrupt(INTERRUPT_LEFT_A,valueChangeOnEncoderLeftPinA,CHANGE);
	attachInterrupt(INTERRUPT_LEFT_B,valueChangeOnEncoderLeftPinB,CHANGE);
	attachInterrupt(INTERRUPT_RIGHT_A,valueChangeOnEncoderRightPinA,CHANGE);
	attachInterrupt(INTERRUPT_RIGHT_B,valueChangeOnEncoderRightPinB,CHANGE);
}

void valueChangeOnEncoderLeftPinA() {
	g_left_encoder.impulseA();
}

void valueChangeOnEncoderLeftPinB() {
	g_left_encoder.impulseB();
}

void valueChangeOnEncoderRightPinA() {
	g_right_encoder.impulseA();
}

void valueChangeOnEncoderRightPinB() {
	g_right_encoder.impulseB();
}
*/

const double DELTA_SPEED_MAX = 500.0 * ENC_MM_TO_TICKS * DUREE_CYCLE / 1000.0;
const double ALPHA_SPEED_MAX = 180.0 * DEG_TO_RAD * DUREE_CYCLE / 1000.0;
void setup()
{
	initSerialLink();

	//attachInterrupts();
	initEncoders();
	
	// LED qui n'en est pas une
	pinMode(16,OUTPUT);
	
	initPWM();
	
	//g_left_encoder.init(INTERRUPT_LEFT_A, PIN_LEFT_A, INTERRUPT_LEFT_B, PIN_LEFT_B);
	//g_right_encoder.init(INTERRUPT_RIGHT_A, PIN_RIGHT_A, INTERRUPT_RIGHT_B, PIN_RIGHT_B);

	/////
	// Par la méthode de ZIEGLER-NICHOLS
	////
	g_delta_regulator
		.setSpeedPID(42.0, 3.3, 0.0)
		.setPosPID(0.01, 0.0, 0.01)
		.setSpeedLimit(DELTA_SPEED_MAX)
		.setAccelLimit(1000.0 * ENC_MM_TO_TICKS * DUREE_CYCLE * DUREE_CYCLE / 1000000.0)
		.setOutLimit(255);
	
	g_alpha_regulator
		.setSpeedPID(40000.0, 500, 500)
		.setPosPID(0.01, 0.005, 1.0)
		.setSpeedLimit(ALPHA_SPEED_MAX)
		.setAccelLimit(360.0 * DEG_TO_RAD * DUREE_CYCLE * DUREE_CYCLE / 1000000.0)
		.setOutLimit(255);
}


bool g_ouch=false;
void loop(){

	int32_t t;
	/*
	if (g_debug_on) {
		for (int x=-30; x<0; ++x) {
			for (int y=-30; y<0; ++y) {
				Serial.print("save ");
				Serial.print(x); Serial.print(' '); Serial.print(y); Serial.print(' ');
				t = micros();
				double hey = atan2(y,x);
				t -= micros();
				Serial.print(-t); Serial.print(' ');
				Serial.println(hey);
			}
		}
		g_debug_on = false;
	}*/
	/* on note le temps de debut */
	int32_t time_start = micros();

	// La del est allumee pendant le traitement
	//digitalWrite(16, HIGH);
	
	/* zone programmation libre */
	t = micros();
	g_observer.compute(G_value_left_enc, G_value_right_enc);
	g_times[6] = micros() - t;
	
	Goal& goal = *(Goal::get());
	int pwm_right=0, pwm_left=0;
	bool reached=false;
	t = micros();
	switch (goal.t()) {
		case Goal::GOAL_POS:
			positionControl(
				goal.x(), goal.y(), goal.a(),
				g_observer.getX(), g_observer.getY(), g_observer.getA(), g_observer.getSpeed(), g_observer.getSpeedA(),
				pwm_left, pwm_right, reached
			);
		break;

		case Goal::GOAL_ANG:
			angleControl(
				goal.x(), goal.y(), goal.a(),
				g_observer.getX(), g_observer.getY(), g_observer.getA(), g_observer.getSpeed(), g_observer.getSpeedA(),
				pwm_left, pwm_right, reached
			);
		break;

		case Goal::GOAL_DELTA_SPEED:
			deltaSpeedControl(goal.speed(), g_observer.getSpeed(), g_delta_regulator, pwm_left, pwm_right);
		break;

		case Goal::GOAL_ALPHA_SPEED:
			alphaSpeedControl(goal.speed_a(), g_observer.getSpeedA(), g_alpha_regulator, pwm_left, pwm_right);
		break;

		case Goal::GOAL_PWM:
			pwm_left = goal.x();
			pwm_right = goal.y();
		break;
		
		default:
			Serial.println("Err : type de goal inconnu");
		break;
	}
	g_times[7] = micros() - t;

	setLeftPWM(pwm_left);
	setRightPWM(pwm_right);

	if (reached and !goal.reached()) {
		goal.setReached(true);
		sendMessage(goal.id(), 0);
	}
	
	readIncomingData();
	
	/* fin zone de programmation libre */

	/* On eteint la del */
	//digitalWrite(16, LOW);

	
	
	/* On attend le temps qu'il faut pour boucler */
	int32_t udelay = DUREE_CYCLE*1000L-(micros()-time_start);
	/*if (g_ouch) {
		for (int i=0; i<g_n; ++i) {
			Serial.print(i); Serial.print(" : "); Serial.println(g_times[i]);
		}
		g_ouch = false;
	}*/
	if(udelay<0) {
		Serial.println("ouch : mainloop trop longue");
		/*for (int i=0; i<g_n; ++i) {
			Serial.print("ouch_"); Serial.print(i); Serial.print(" : "); Serial.println(g_times[i]);
		}
		g_ouch = true;*/
	}
	else if (udelay > 1000) {
		delay(udelay/1000);
	}
	else {
		delayMicroseconds(udelay);
	}
}


