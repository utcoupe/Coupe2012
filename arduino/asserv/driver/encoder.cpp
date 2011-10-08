/*
 * encoder.cpp
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */
#include "encoder.h"
#include "wiring.h"

int state_left_pinA;
int state_left_pinB;
int state_right_pinA;
int state_right_pinB;

long value_left_enc;
long value_right_enc;

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

	attachInterrupt(INTERRUPT_LEFT_A,valueChangeOnEncoderLeftPinA,CHANGE);
	attachInterrupt(INTERRUPT_LEFT_B,valueChangeOnEncoderLeftPinB,CHANGE);
	attachInterrupt(INTERRUPT_RIGHT_A,valueChangeOnEncoderRightPinA,CHANGE);
	attachInterrupt(INTERRUPT_RIGHT_B,valueChangeOnEncoderRightPinB,CHANGE);
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

