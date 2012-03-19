#include "balais.h"

#include "WProgram.h"
#include "message.h"

Balais g_balais;

Balais::Balais() {
	this->pwm = 0;
	this->sens = FORWARD;
	this->motors = (AF_DCMotor*)malloc(sizeof(AF_DCMotor) * NB_MOTORS_BALAIS);
	for (int i=0; i<NB_MOTORS_BALAIS; ++i) {
		this->motors[i] = AF_DCMotor(i+1, MOTOR12_64KHZ);
		this->motors[i].setSpeed(0);
		this->motors[i].run(FORWARD);
	}
}


int Balais::set_pwm(int _pwm) {
	this->pwm = min(255,abs(_pwm));
	if (_pwm < 0) {
		this->sens = BACKWARD;
	}
	else {
		this->sens = FORWARD;
	}
	
	for (int i=0; i<NB_MOTORS_BALAIS; ++i) {
		this->motors[i].setSpeed(this->pwm);
	}

	return 1;
}


