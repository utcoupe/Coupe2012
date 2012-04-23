#include "motor.h"

#include "tools.h"

AF_DCMotor moteurG;
AF_DCMotor moteurD;

void initPWM(){
	moteurG = AF_DCMotor(2, MOTOR12_64KHZ); // A
	moteurD = AF_DCMotor(1, MOTOR12_64KHZ); // B
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
	if(value>0)
		moteurD.run(FORWARD);
	else
		moteurD.run(BACKWARD);
}
