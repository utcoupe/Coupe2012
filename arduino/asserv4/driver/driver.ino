#include "include_arduino.h"


#include "parameters.h"
#include <math.h>

#include "encoder.h"
#include "robotstate.h"
#include "pwm.h"
#include "message.h"
#include "control.h"
#include "goal.h"
#include "globals.h"

unsigned long index = 0;
unsigned long timeStart = 0;

void setup(){
	value_pwm_left = 0;
	value_pwm_right = 0;
	
	/*Active les pwm*/
	initPWM();
	/*Initialise le regulateur*/
	initController();
	/*Active les interruptions sur les encodeurs*/
	initEncoders();
	/*Definit la position initiale du robot*/
	initRobotState();
	/*Active la liaison serie*/
	initSerialLink();

	// LED qui n'en est pas une
	pinMode(16,OUTPUT);
}

void loop(){
	/* on note le temps de debut */
	timeStart = micros();

	/* La del est allumee pendant le traitement */
	digitalWrite(16, HIGH);

	/* zone programmation libre */
	
	g_observer.compute(value_left_enc, value_right_enc);
	//computeRobotState();
	
	Goal& goal = *(Goal::get());
	int pwm_right=0, pwm_left=0;
	bool reached=false;
	if (!g_stop) {
		switch (goal.t()) {
			
			case Goal::GOAL_POS:
				/*positionControl(
					goal.x(), goal.y(), goal.a(),
					g_observer.getX(), g_observer.getY(), g_observer.getA(), g_observer.getSpeed(), g_observer.getSpeedA(),
					pwm_left, pwm_right, reached
				);*/
			break;

			case Goal::GOAL_ANG:
				/*angleControl(
					goal.x(), goal.y(), goal.a(),
					g_observer.getX(), g_observer.getY(), g_observer.getA(), g_observer.getSpeed(), g_observer.getSpeedA(),
					pwm_left, pwm_right, reached
				);*/
			break;

			case Goal::GOAL_DELTA_SPEED:
				//deltaSpeedControl(goal.speed(), g_observer.getSpeed(), g_delta_regulator, pwm_left, pwm_right);
			break;

			case Goal::GOAL_ALPHA_SPEED:
				//alphaSpeedControl(goal.speed_a(), g_observer.getSpeedA(), g_alpha_regulator, pwm_left, pwm_right);
			break;
			

			case Goal::GOAL_PWM:
				pwm_left = goal.x();
				pwm_right = goal.y();
			break;
			
			default:
				Serial.println("Err : type de goal inconnu");
			break;
		}
	}

	setLeftPWM(pwm_left);
	setRightPWM(pwm_right);

	if (reached and !goal.reached()) {
		goal.setReached(true);
		sendMessage(goal.id(), 0);
	}
	
	readIncomingData();
	
	/* fin zone de programmation libre */
	
	/* On eteint la del */
	digitalWrite(16, LOW);
	
	/* On attend le temps qu'il faut pour boucler */
	long udelay = DUREE_CYCLE*1000-(micros()-timeStart);
	if(udelay<0)
		Serial.println("ouch : mainloop trop longue");
	else
		 delayMicroseconds(udelay);
}


