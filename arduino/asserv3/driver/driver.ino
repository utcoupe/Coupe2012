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

double alpha_diff(double a, double b)
{
	//return atan2(sin(a-b), cos(a-b));
	return moduloPI(a - b);
}

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
	
	g_delta_regulator
		.setSpeedPID(200.0, 0, 10.0)
		.setPosPID(0.01, 0.0, 0.01)
		.setSpeedLimit(500.0 * ENC_MM_TO_TICKS * DUREE_CYCLE / 1000.0)
		.setAccelLimit(1000.0 * ENC_MM_TO_TICKS * DUREE_CYCLE * DUREE_CYCLE / 1000000.0)
		.setOutLimit(255);
	
	g_alpha_regulator
		.setSpeedPID(500000.0, 0, 500000.0)
		.setPosPID(0.01, 0, 0.0)
		.setSpeedLimit(90.0 * DEG_TO_RAD * DUREE_CYCLE / 1000.0)
		.setAccelLimit(360.0 * DEG_TO_RAD * DUREE_CYCLE * DUREE_CYCLE / 1000000.0)
		.setOutLimit(255);
}

int32_t last;
void loop(){
	/* on note le temps de debut */
	int32_t time_start = micros();

	// La del est allumee pendant le traitement
	digitalWrite(16, HIGH);
	
	/* zone programmation libre */
	
	g_observer.compute(G_value_left_enc, G_value_right_enc);

	
	int pwm_left = 0;
	int pwm_right = 0;
	int output4Delta = 0;
	int output4Alpha = 0;
	
	double alpha_diff_alpha = alpha_diff(g_goal_a, g_observer.getA());
	double alpha_diff_delta = alpha_diff(atan2(g_goal_y-g_observer.getY(), g_goal_x-g_observer.getX()), g_observer.getA());
	
	double alpha_diff = 0.0;
	switch (g_goal_type)
	{
		case GOAL_POS:
			alpha_diff = alpha_diff_delta;
		break;

		case GOAL_ANG:
			alpha_diff = alpha_diff_alpha;
		break;
	}

	int sens_delta=1;
	// Si le goal est derrière
	if(abs(alpha_diff_delta) > M_PI/2) {
		sens_delta = -sens_delta;
		if (g_goal_type == GOAL_POS) {
			alpha_diff = moduloPI(M_PI + alpha_diff);
		}
	}
	
	int32_t dx = g_goal_x - g_observer.getX();
	int32_t dy = g_goal_y - g_observer.getY();

	double delta_diff = sqrt(dx*dx+dy*dy);
	double current_delta = sens_delta * delta_diff;
	double current_alpha = alpha_diff;

	if ((g_goal_type == GOAL_ANG and fabs(alpha_diff) < DEG_TO_RAD) or
		(g_goal_type == GOAL_POS and fabs(delta_diff) < 10*ENC_MM_TO_TICKS))
	{
		// goal atteind
	}
	else {
		output4Delta = (int) g_delta_regulator.compute(0.0, (double)-current_delta, g_observer.getSpeed());
		output4Alpha = (int) g_alpha_regulator.compute(0.0, (double)-current_alpha, g_observer.getSpeedA());

		pwm_left = output4Delta-output4Alpha;
		pwm_right = output4Delta+output4Alpha;
		pwm_left = min(255, max(-255, pwm_left));
		pwm_right = min(255, max(-255, pwm_right));
	}
	
	setLeftPWM(pwm_left);
	setRightPWM(pwm_right);
	
	if (g_debug_on and (time_start - last) > 50000) {
		Serial.println("--- coucou --");
		//Serial.print(g_left_encoder.getValue()); Serial.print(" "); Serial.println(g_right_encoder.getValue());
		Serial.print(G_value_left_enc); Serial.print(" "); Serial.println(G_value_right_enc);
		/*Serial.print(g_observer.mm_getX()); Serial.print(" ");
		Serial.print(g_observer.mm_getY()); Serial.print(" ");
		Serial.print(g_observer.deg_getA()); Serial.println(" ");*/
		Serial.print("pwm_left/right="); Serial.print(pwm_left); Serial.print(" "); Serial.println(pwm_right);
		/*Serial.print("output delta/alpha="); Serial.print(output4Delta); Serial.print(" "); Serial.println(output4Alpha);
		Serial.print("speed="); Serial.println(g_observer.getSpeed());
		Serial.print("speed_consign="); Serial.println(g_delta_regulator.getSpeedConsign());
		Serial.print("current_delta="); Serial.println(current_delta);*/
		Serial.print("plot ");
		Serial.print(millis()); Serial.print(" ");
		/*Serial.print(g_alpha_regulator.getSpeedConsign()*100000.0); Serial.print(" "); // bleu
		Serial.print(g_observer.getSpeedA()*100000.0); Serial.print(" "); // verte
		Serial.println(pwm_left); // rouge*/
		Serial.print(g_delta_regulator.getSpeedConsign()*100); Serial.print(" "); // bleu
		Serial.print(g_observer.getSpeed()*100); Serial.print(" "); // verte
		Serial.println(pwm_left); // rouge

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


