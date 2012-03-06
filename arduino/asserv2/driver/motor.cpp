#include "motor.h"

#include "tools.h"

AF_DCMotor moteurG(1, MOTOR12_64KHZ);
AF_DCMotor moteurD(2, MOTOR12_64KHZ);


void initPWM(){
	moteurG = AF_DCMotor(1, MOTOR12_64KHZ);
	moteurD = AF_DCMotor(2, MOTOR12_64KHZ);
	moteurG.setSpeed(0);
	moteurG.run(FORWARD);
	moteurD.setSpeed(0);
	moteurD.run(FORWARD);
}

void setLeftPWM(int value){
	moteurG.setSpeed(abs(value));
	if(value<0)
		moteurG.run(FORWARD);
	else
		moteurG.run(BACKWARD);
}

void setRightPWM(int value){
	moteurD.setSpeed(abs(value));
	if(value<0)
		moteurD.run(FORWARD);
	else
		moteurD.run(BACKWARD);
}
