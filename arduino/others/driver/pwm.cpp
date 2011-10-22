/*
 * pwm.cpp
 *
 *  Created on: 13 janv. 2011
 *      Author: HoHen
 */
#include "wiring.h"
#include "pwm.h"
#include "AFMotor.h"

AF_DCMotor MoteurG(1, MOTOR12_64KHZ); //A
AF_DCMotor MoteurD(2, MOTOR12_64KHZ); //B

void initPWM(){
	MoteurG = AF_DCMotor(1, MOTOR12_64KHZ);
	MoteurD = AF_DCMotor(2, MOTOR12_64KHZ);
	MoteurG.setSpeed(0);
	MoteurG.run(FORWARD);
	MoteurD.setSpeed(0);
	MoteurD.run(FORWARD);
}

void setAVPWM(int value){
	if(value<0)
		MoteurG.run(FORWARD);
	else
		MoteurG.run(BACKWARD);
	MoteurG.setSpeed(abs(value));
}

void setARPWM(int value){
	if(value<0)
		MoteurD.run(FORWARD);
	else
		MoteurD.run(BACKWARD);
	MoteurD.setSpeed(abs(value));
}
