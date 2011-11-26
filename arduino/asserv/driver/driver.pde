#include "WProgram.h"
#include "parameters.h"
#include <math.h>

#include "encoder.h"
#include "robotstate.h"
#include "pwm.h"
#include "fifo.h"
#include "message.h"
#include "control.h"

unsigned long index = 0;
unsigned long timeStart = 0;

void setup(){
	value_pwm_left = 0;
	value_pwm_right = 0;
	/*Initialise la file des buts a atteindre*/
	initGoals();
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

	/* zone programmation libre */
	// La del est allumee pendant le traitement
	digitalWrite(16, HIGH);
	/* fin zone de programmation libre */

	/*lecture des ordres*/
	readIncomingData();

	/*recuperation du but suivant (vitesse, angle ou position) */
	if(current_goal.isReached)
		popGoal(); /* va changer la valeur de current_goal */


	/*traitement des taches*/
	if(!current_goal.isReached){
		if(current_goal.type == TYPE_SPEED)
			speedControl(&value_pwm_left,&value_pwm_right);
		else if(current_goal.type == TYPE_ANGLE)
			angleControl(&value_pwm_left,&value_pwm_right);
		else if(current_goal.type == TYPE_POSITION)
			positionControl(&value_pwm_left,&value_pwm_right);
		else if(current_goal.type == TYPE_CALIB_X){
			robot_state.x = current_goal.x;
			current_goal.isReached = true;
		}
		else if(current_goal.type == TYPE_CALIB_Y){
			robot_state.y = current_goal.y;
			current_goal.isReached = true;
		}
		else if(current_goal.type == TYPE_CALIB_ANGLE){
			robot_state.angle = current_goal.angle;
			current_goal.isReached = true;
		}
		else if(current_goal.type == TYPE_DELAY)
			delayControl(&value_pwm_left,&value_pwm_right);
		else if(current_goal.type == TYPE_PWM)
			pwmControl(&value_pwm_left,&value_pwm_right);
	}

	/*ecriture de la sortie*/
	setLeftPWM(value_pwm_left);
	setRightPWM(value_pwm_right);

	/*modele d'evolution*/
	computeRobotState();
	
	/* On eteint la del */
	digitalWrite(16, LOW);
	
	/* On attend le temps qu'il faut pour boucler */
	long udelay = DUREE_CYCLE*1000-(micros()-timeStart);
	if(udelay>0) delayMicroseconds(udelay);
}


