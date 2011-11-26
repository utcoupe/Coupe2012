/*
 * fifo.c
 *
 *  Created on: 16 janv. 2011
 *	  Author: HoHen
 */

#include "fifo.h"
#include "math.h"
#include "wiring.h"

Fifo goals;

void initGoals(){
	goals.goal = (Goal*)malloc(sizeof(Goal)*SIZE);
	goals.in = 0;
	goals.out = 0;
	
	//pushGoalAutoCalibration(0,0);
	/*pour tester le robot
	pushGoalOrientation(3.14,255); //angle,vitesse
	pushGoalPosition(10000,9000,255); //x,y,vitesse
	pushGoalSpeed(2.0,10000); //vitesse,periode
	*/
	//pushGoalOrientation(3.14,255);
	/*pushGoalDelay(1000);
	pushGoalOrientation(3.14,255);
	pushGoalDelay(1000);
	pushGoalOrientation(3*3.14/2,255);
	pushGoalDelay(1000);
	pushGoalOrientation(0,255);*/
	//pushGoalOrientation(1.414,200);


}

void pushGoalPosition(int id, double x, double y, double speed){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_POSITION;
		incGoal->id = id;
		incGoal->data_1 = x;
		incGoal->data_2 = y;
		incGoal->data_3 = speed;
		goals.in = (goals.in+1)%SIZE;
	}
}

void pushGoalOrientation(int id, double angle, double speed){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_ANGLE;
		incGoal->id = id;
		incGoal->data_1 = angle;
		incGoal->data_2 = speed;
		goals.in = (goals.in+1)%SIZE;
	}
}

void pushGoalSpeed(int id, double speed, double period){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_SPEED;
		incGoal->id = id;
		incGoal->data_1 = speed;
		incGoal->data_2 = period;
		goals.in = (goals.in+1)%SIZE;
	}
}

void pushGoalPwm(int id, double speed, double period){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_PWM;
		incGoal->id = id;
		incGoal->data_1 = speed;
		incGoal->data_2 = period;
		goals.in = (goals.in+1)%SIZE;
	}
}

void pushGoalManualCalibration(int type,double value){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = type;
		incGoal->data_1 = value;
		incGoal->id = NO_ID;
		goals.in = (goals.in+1)%SIZE;
	}
}

void pushGoalDelay(double value){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_DELAY;
		incGoal->data_1 = value;
		incGoal->id = NO_ID;
		goals.in = (goals.in+1)%SIZE;
	}
}

void pushGoalAutoCalibration(int id, bool color){ /* false -> blue / true -> red */
	const int SPEED_DELTA=150, SPEED_ANGL=150, PWM=-70;
	if(!color){
		/* phase 0 : on fixe les valeurs de l'etat */
		pushGoalManualCalibration(TYPE_CALIB_X,0);
		pushGoalManualCalibration(TYPE_CALIB_Y,0);
		pushGoalManualCalibration(TYPE_CALIB_ANGLE,0);
		/* phase 1 : tourner en PI/2 */
		pushGoalOrientation(NO_ID,M_PI/2,SPEED_ANGL);
		/* phase 2 : reculer pendant 2s */
		pushGoalPwm(NO_ID,PWM,2000);
		/* phase 3 : fixer X et angle */
		pushGoalManualCalibration(TYPE_CALIB_Y,DIST_MOTOR_AXIS_TO_BACK_MM*ENC_MM_TO_TICKS); //c'est 120 mm (distance entre l'axe des moteurs et le derriere du robot)
		pushGoalManualCalibration(TYPE_CALIB_ANGLE,M_PI/2);
		/* phase 4 : avancer un peu pour pouvoir tourner */
		pushGoalPosition(NO_ID,0,200.0*ENC_MM_TO_TICKS,SPEED_DELTA);
		/* phase 5 : tourner en 0 */
		pushGoalOrientation(NO_ID,0,SPEED_ANGL);
		/* phase 6 : reculer pendant 2s */
		pushGoalPwm(NO_ID,PWM,2000);
		/* phase 7 : fixer Y (et peut-etre speed, a voir si c'est utile) */
		pushGoalManualCalibration(TYPE_CALIB_X,DIST_MOTOR_AXIS_TO_BACK_MM*ENC_MM_TO_TICKS); //c'est 120 mm (distance entre l'axe des moteurs et le derriere du robot)
		/* phase 8 : avancer de quelques cm */
		pushGoalPosition(NO_ID,(390.0-DIST_MOTOR_AXIS_TO_BACK_MM)*ENC_MM_TO_TICKS,200.0*ENC_MM_TO_TICKS,SPEED_DELTA);
		/* phase 9 : reorientation exact */
		pushGoalOrientation(id,0,SPEED_ANGL);
	}
	else{
		/* phase 0 : on fixe les valeurs de l'etat */
		pushGoalManualCalibration(TYPE_CALIB_X,0);
		pushGoalManualCalibration(TYPE_CALIB_Y,0);
		pushGoalManualCalibration(TYPE_CALIB_ANGLE,M_PI);
		/* phase 1 : tourner d'un angle PI/2 */
		pushGoalOrientation(NO_ID,M_PI/2,SPEED_ANGL);
		/* phase 2 : reculer pendant 2s */
		pushGoalPwm(NO_ID,PWM,2000);
		/* phase 3 : fixer X et angle */
		pushGoalManualCalibration(TYPE_CALIB_Y,DIST_MOTOR_AXIS_TO_BACK_MM*ENC_MM_TO_TICKS);
		pushGoalManualCalibration(TYPE_CALIB_ANGLE,M_PI/2);
		/* phase 4 : avancer un peu pour pouvoir tourner */
		pushGoalPosition(NO_ID,0,200.0*ENC_MM_TO_TICKS,SPEED_DELTA);
		/* phase 5 : tourner en PI */
		pushGoalOrientation(NO_ID,M_PI,SPEED_ANGL);
		/* phase 6 : reculer pendant 2s */
		pushGoalPwm(NO_ID,PWM,2000);
		/* phase 7 : fixer Y (et peut-etre speed, a voir si c'est utile) */
		pushGoalManualCalibration(TYPE_CALIB_X,(TABLE_HEIGHT_MM-DIST_MOTOR_AXIS_TO_BACK_MM)*ENC_MM_TO_TICKS);
		/* phase 8 : avancer de quelques cm */
		pushGoalPosition(NO_ID,(2610.0+DIST_MOTOR_AXIS_TO_BACK_MM)*ENC_MM_TO_TICKS,200.0*ENC_MM_TO_TICKS,SPEED_DELTA);
		/* phase 9 : reorientation exact */
		pushGoalOrientation(id,M_PI,SPEED_ANGL);
	}
}




void popGoal(){
	if(goals.in!=goals.out){
		current_goal.isReached = false;
		current_goal.isCanceled = false;
		current_goal.isMessageSent = false;
		current_goal.phase = PHASE_1;
		Goal* outGoal = goals.goal+goals.out;
		current_goal.type = outGoal->type;
		current_goal.id = outGoal->id;
		switch (outGoal->type) {
		case TYPE_SPEED:
			current_goal.speed = outGoal->data_1;
			current_goal.period = outGoal->data_2;
			break;
		case TYPE_ANGLE:
			current_goal.angle = outGoal->data_1;
			current_goal.speed = outGoal->data_2;
			break;
		case TYPE_POSITION:
			current_goal.x = outGoal->data_1;
			current_goal.y = outGoal->data_2;
			current_goal.speed = outGoal->data_3;
			break;
		case TYPE_PWM:
			current_goal.speed = outGoal->data_1;
			current_goal.period = outGoal->data_2;
			break;
		case TYPE_DELAY:
			current_goal.period = outGoal->data_1;
			break;
		case TYPE_CALIB_X:
			current_goal.x = outGoal->data_1;
			break;
		case TYPE_CALIB_Y:
			current_goal.y = outGoal->data_1;
			break;
		case TYPE_CALIB_ANGLE:
			current_goal.angle = outGoal->data_1;
			break;
		default:
			break;
		}
		goals.out = (goals.out+1)%SIZE;
	}
}

void clearGoals(){
	goals.in = 0;
	goals.out = 0;
}

bool fifoIsEmpty(){
	return goals.in==goals.out;
}
