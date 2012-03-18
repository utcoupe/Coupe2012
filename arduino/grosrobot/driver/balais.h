#ifndef __BALAIS_H__
#define __BALAIS_H__



#include "AFMotor.h"

#define NB_MOTORS_BALAIS		2



class Balais {

	public:
		Balais();
		int set_pwm(int pwm);

	private:
		int pwm;
		int sens;
		AF_DCMotor * motors;
	
};



extern Balais g_balais;



#endif


