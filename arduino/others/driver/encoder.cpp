/*
 * encoder.cpp
 *
 *  Created on: 13 janv. 2011
 *      Author: HoHen
 */
#include "encoder.h"

int state_left_pinA;
int state_left_pinB;
int state_right_pinA;
int state_right_pinB;

long value_left_enc;//AR
long value_right_enc;//AV
long goal_position_AV,goal_position_AR;

void testAV()
{
	if (goal_position_AV < 0)
		return;
	if (value_right_enc < goal_position_AV-MARGE_MARCHE)
		setAVPWM(PWM_MOVE);
	else if (value_right_enc < goal_position_AV-MARGE_MAINTIENT)
		setAVPWM(PWM_MAINTIENT);
	else if (value_right_enc > goal_position_AV+MARGE_MARCHE)
		setAVPWM(-PWM_MOVE);
	else if (value_right_enc > goal_position_AV+MARGE_MAINTIENT)
		setAVPWM(-PWM_MAINTIENT);
	else //arret
	{
		setAVPWM(0x00);
		if(msg_position_AV!=-1){
			sendMessage(msg_position_AV,2);
			msg_position_AV=-1;
		}
	}
}

void testAR()
{
	if (goal_position_AR < 0)
		return;
	if (value_left_enc < goal_position_AR-MARGE_MARCHE)
		setARPWM(PWM_MOVE);
	else if (value_left_enc < goal_position_AR-MARGE_MAINTIENT)
		setARPWM(PWM_MAINTIENT);
	else if (value_left_enc > goal_position_AR+MARGE_MARCHE)
		setARPWM(-PWM_MOVE);
	else if (value_left_enc > goal_position_AR+MARGE_MAINTIENT)
		setARPWM(-PWM_MAINTIENT);
	else //arret
	{
		setARPWM(0x00);
		if(msg_position_AR!=-1){
			sendMessage(msg_position_AR,2);
			msg_position_AR=-1;
		}
	}
}

/**
 * Test si les ascenseurs bloquent
 * Envoie le message E_BLOCK si un ascneseur bloque
 */
void encoderSafe()
{
	/*static int last_left_enc_value = 0;
	//static int last_right_enc_value = 0;
	
	if (goal_position_AR >= 0
		and ((value_left_enc < goal_position_AR-MARGE_MAINTIENT)
		or (value_left_enc > goal_position_AR+MARGE_MAINTIENT)))
	{
		if (abs(last_left_enc_value - value_left_enc) < 10)
		{
			goal_position_AV = -1;
			setARPWM(0x00);
			sendMessage(E_BLOCK,E_BLOCK);
		}
	}*/
	
	/*if (goal_position_AV >= 0
		and ((value_right_enc < goal_position_AV-MARGE_MAINTIENT)
		or (value_right_enc > goal_position_AV+MARGE_MAINTIENT)))
	{
		if (abs(last_right_enc_value - value_right_enc) < 10)
		{
			goal_position_AR = -1;
			setAVPWM(0x00);
			sendMessage(E_BLOCK,E_BLOCK);
		}
	}*/
}

void initEncoders(){
	value_left_enc = 0;
	value_right_enc = 0;

	pinMode(PIN_LEFT_A,INPUT);
	pinMode(PIN_LEFT_B,INPUT);
	pinMode(PIN_RIGHT_A,INPUT);
	pinMode(PIN_RIGHT_B,INPUT);

	digitalWrite(PIN_LEFT_A, HIGH);
	digitalWrite(PIN_LEFT_B, HIGH);
	digitalWrite(PIN_RIGHT_A, HIGH);
	digitalWrite(PIN_RIGHT_B, HIGH);

	state_left_pinA = digitalRead(PIN_LEFT_A);
	state_left_pinB = digitalRead(PIN_LEFT_B);
	state_right_pinA = digitalRead(PIN_RIGHT_A);
	state_right_pinB = digitalRead(PIN_RIGHT_B);

	//attachInterrupt(INTERRUPT_LEFT_A,valueChangeOnEncoderLeftPinA,CHANGE);
	//attachInterrupt(INTERRUPT_LEFT_B,valueChangeOnEncoderLeftPinB,CHANGE);
	//attachInterrupt(INTERRUPT_RIGHT_A,valueChangeOnEncoderRightPinA,CHANGE);
	//attachInterrupt(INTERRUPT_RIGHT_B,valueChangeOnEncoderRightPinB,CHANGE);
	attachInterrupt(INTERRUPT_LEFT_A,valueChangeOnMSRecalARHaut,CHANGE);
	attachInterrupt(INTERRUPT_LEFT_B,valueChangeOnMSRecalARBas,CHANGE);
	attachInterrupt(INTERRUPT_RIGHT_A,valueChangeOnMSRecalAVHaut,CHANGE);
	attachInterrupt(INTERRUPT_RIGHT_B,valueChangeOnMSRecalAVBas,CHANGE);
}


void valueChangeOnEncoderLeftPinA(){
	int new_state = digitalRead(PIN_LEFT_A);

	if(new_state == HIGH)
		if(state_left_pinB == HIGH)
			value_left_enc--;
		else
			value_left_enc++;

	else
		if(state_left_pinB == HIGH)
			value_left_enc++;
		else
			value_left_enc--;

	state_left_pinA = new_state;
	testAR();
}

void valueChangeOnEncoderLeftPinB(){
	int new_state = digitalRead(PIN_LEFT_B);

	if(new_state == HIGH)
		if(state_left_pinA == HIGH)
			value_left_enc++;
		else
			value_left_enc--;

	else
		if(state_left_pinA == HIGH)
			value_left_enc--;
		else
			value_left_enc++;

	state_left_pinB = new_state;
}

void valueChangeOnEncoderRightPinA(){
	int new_state = digitalRead(PIN_RIGHT_A);

	if(new_state == HIGH)
		if(state_right_pinB == HIGH)
			value_right_enc--;
		else
			value_right_enc++;

	else
		if(state_right_pinB == HIGH)
			value_right_enc++;
		else
			value_right_enc--;

	state_right_pinA = new_state;
	testAV();
}

void valueChangeOnEncoderRightPinB(){
	int new_state = digitalRead(PIN_RIGHT_B);

	if(new_state == HIGH)
		if(state_right_pinA == HIGH)
			value_right_enc++;
		else
			value_right_enc--;

	else
		if(state_right_pinA == HIGH)
			value_right_enc--;
		else
			value_right_enc++;

	state_right_pinB = new_state;
}


void valueChangeOnMSRecalAVBas()
{
	delay(500);
	
	if (digitalRead(PIN_MS_RECAL_AV_BAS) == HIGH)
	{
		value_right_enc = 0;
		sendMessage(-101, (char*)"av bas");
		setAVPWM(0x00);
		if(msg_position_AV != -1){
			sendMessage(msg_position_AV,2);
			msg_position_AV=-1;
		}
	}
}


void valueChangeOnMSRecalAVHaut()
{
	delay(500);
	
	if (digitalRead(PIN_MS_RECAL_AV_HAUT) == HIGH)
	{
		value_right_enc = POSITION_MAX;
		sendMessage(-101, (char*)"av haut");
		setAVPWM(0x00);
		if(msg_position_AV != -1){
			sendMessage(msg_position_AV,2);
			msg_position_AV=-1;
		}
	}
}


void valueChangeOnMSRecalARBas()
{
	delay(500);
	
	if (digitalRead(PIN_MS_RECAL_AR_BAS) == HIGH)
	{
		value_left_enc = 0;
		sendMessage(-101, (char*)"ar bas");
		setARPWM(0x00);
		if(msg_position_AR != -1){
			sendMessage(msg_position_AR,2);
			msg_position_AR=-1;
		}
	}
}


void valueChangeOnMSRecalARHaut()
{
	delay(500);
	
	if (digitalRead(PIN_MS_RECAL_AR_HAUT) == HIGH)
	{
		value_left_enc = POSITION_MAX;
		sendMessage(-101, (char*)"ar haut");
		setARPWM(0x00);
		if(msg_position_AR != -1){
			sendMessage(msg_position_AR,2);
			msg_position_AR=-1;
		}
	}
}
