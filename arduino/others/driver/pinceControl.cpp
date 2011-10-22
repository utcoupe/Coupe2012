#include "pinceControl.h"
//#include "servoControl.h"

int msg_position_AV,msg_position_AR;

void initPinceControl()
{
	goal_position_AV=-1;
	goal_position_AR=-1;
	initPWM();
	//initServo();
}

int pinceRecal(int id, char face)
{
	sendMessage(id, 1);
	pinMode(PIN_MS_RECAL_AV, OUTPUT);
	digitalWrite(PIN_MS_RECAL_AV, LOW);
	pinMode(PIN_MS_RECAL_AR, OUTPUT);
	digitalWrite(PIN_MS_RECAL_AR, LOW);
	pinMode(PIN_MS_RECAL_AV,INPUT);
	pinMode(PIN_MS_RECAL_AR,INPUT);
	goal_position_AV=-1;
	goal_position_AR=-1;
	if(face==AVANT){
		//while(digitalRead(PIN_MS_RECAL_AV)!=HIGH)
		while(digitalRead(PIN_MS_RECAL_AV_BAS)!=HIGH)
		{
			setAVPWM(-PWM_MAINTIENT);
			delay(40);
		}
		setAVPWM(0x00);
		initEncoders();
		return 2;
	}
	if(face==ARRIERE){
		//while(digitalRead(PIN_MS_RECAL_AR)!=HIGH){
		while(digitalRead(PIN_MS_RECAL_AR_BAS)!=HIGH)
		{
			setARPWM(-PWM_MAINTIENT);
			delay(40);
		}
		setARPWM(0x00);
		initEncoders();
		return 2;
	}
	return -42;
}

/** Utilisation des servos
 */
/*
int setPinceState( char index, char etat){
	return setServoState(index, etat);
}*/

int setPincePosition(int id, char index, int pos)
{
	int pwm=PWM_MOVE;
	int pos_in_ticks=0;
	switch (pos)
	{
		case HAUT:
			pos_in_ticks = POSITION_MAX;
		break;

		case MIDLE:
			pos_in_ticks = 4000;
		break;

		case BAS:
			pos_in_ticks = 0;
		break;

		default:
			return E_INVALID_PARAMETERS_VALUE;
		break;
	}
	
	switch(index)
	{
		case AVANT:
			if (goal_position_AV != pos_in_ticks)
			{
				goal_position_AV=pos_in_ticks;
				msg_position_AV=id;
				return 1;
			}
			else
			{
				sendMessage(id, 1);
				return 2;
			}
		break;
		
		case ARRIERE:
			/*goal_position_AR=pos_in_ticks;
			msg_position_AR=id;
			testAR();
			return 1;*/
			if (goal_position_AR != pos_in_ticks)
			{
				goal_position_AR=pos_in_ticks;
				msg_position_AR=id;
				return 1;
			}
			else
			{
				sendMessage(id, 1);
				return 2;
			}
		break;
		
		default : 
			return E_INVALID_PARAMETERS_VALUE;
		break;
	}
}

