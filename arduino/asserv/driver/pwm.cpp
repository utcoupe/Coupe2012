/*
 * pwm.cpp
 *
 *  Created on: 13 janv. 2011
 *      Author: HoHen
 */
#include "wiring.h"
#include "pwm.h"
#include "AFMotor.h"

AF_DCMotor MoteurG(1, MOTOR12_64KHZ);
AF_DCMotor MoteurD(2, MOTOR12_64KHZ);

void initPWM(){
	MoteurG = AF_DCMotor(1, MOTOR12_64KHZ);
	MoteurD = AF_DCMotor(2, MOTOR12_64KHZ);
	MoteurG.setSpeed(0);
	MoteurG.run(FORWARD);
	MoteurD.setSpeed(0);
	MoteurD.run(FORWARD);
}

void setLeftPWM(int value){
	MoteurG.setSpeed(abs(value));
	if(value<0)
		MoteurG.run(FORWARD);
	else
		MoteurG.run(BACKWARD);
}

void setRightPWM(int value){
	MoteurD.setSpeed(abs(value));
	if(value>0)
		MoteurD.run(FORWARD);
	else
		MoteurD.run(BACKWARD);
}

