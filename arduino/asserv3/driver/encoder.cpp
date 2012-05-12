#include "encoder.h"
#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#	include "wiring.h"
#endif

#include "parameters.h"



int state_left_pinA;
int state_left_pinB;
int state_right_pinA;
int state_right_pinB;

long G_value_left_enc;
long G_value_right_enc;

void initEncoders(){
	G_value_left_enc = 0;
	G_value_right_enc = 0;

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
			G_value_left_enc--;
		else
			G_value_left_enc++;

	else
		if(state_left_pinB == HIGH)
			G_value_left_enc++;
		else
			G_value_left_enc--;

	state_left_pinA = new_state;
}

void valueChangeOnEncoderLeftPinB(){
	int new_state = digitalRead(PIN_LEFT_B);

	if(new_state == HIGH)
		if(state_left_pinA == HIGH)
			G_value_left_enc++;
		else
			G_value_left_enc--;

	else
		if(state_left_pinA == HIGH)
			G_value_left_enc--;
		else
			G_value_left_enc++;

	state_left_pinB = new_state;
}

void valueChangeOnEncoderRightPinA(){
	int new_state = digitalRead(PIN_RIGHT_A);

	if(new_state == HIGH)
		if(state_right_pinB == HIGH)
			G_value_right_enc--;
		else
			G_value_right_enc++;

	else
		if(state_right_pinB == HIGH)
			G_value_right_enc++;
		else
			G_value_right_enc--;

	state_right_pinA = new_state;
}

void valueChangeOnEncoderRightPinB(){
	int new_state = digitalRead(PIN_RIGHT_B);

	if(new_state == HIGH)
		if(state_right_pinA == HIGH)
			G_value_right_enc++;
		else
			G_value_right_enc--;

	else
		if(state_right_pinA == HIGH)
			G_value_right_enc--;
		else
			G_value_right_enc++;

	state_right_pinB = new_state;
}

/*

#include "encoder.h"
#include "Arduino.h"



Encoder::Encoder() : _value(0), _state_a(0), _state_b(0), _pin_a(0) {};

void Encoder::init(int interruptA, int pinA, int interruptB, int pinB) {
	pinMode(pinA, INPUT);
	pinMode(pinB, INPUT);
	digitalWrite(pinA, HIGH);
	digitalWrite(pinB, HIGH);
	_state_a = digitalRead(pinA);
	_state_b = digitalRead(pinB);
	_pin_a = pinA;
	_pin_b = pinB;
}

void Encoder::impulseA() {
	_state_a = digitalRead(_pin_a);

	if(_state_a == HIGH)
		if(_state_b == HIGH)
			_value--;
		else
			_value++;

	else
		if(_state_b == HIGH)
			_value++;
		else
			_value--;
}

void Encoder::impulseB() {
	_state_b = digitalRead(_pin_b);

	if(_state_b == HIGH)
		if(_state_a == HIGH)
			_value++;
		else
			_value--;

	else
		if(_state_a == HIGH)
			_value--;
		else
			_value++;
}


int32_t Encoder::getValue() const {
	return _value;
}


*/
