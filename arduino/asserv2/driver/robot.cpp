#include "robot.h"
#include "encoder.h"
#include "WProgram.h"
#include "pid.h"
#include "tools.h"

#include <math.h>


/**
 * a-b
 */
double angle_diff(double a, double b)
{
	//return atan2(sin(a-b), cos(a-b));
	return moduloPI(a - b);
}



Robot robot(&value_left_enc, &value_right_enc, &moteurG, &moteurD);

Rampe rampe_delta = Rampe();
Rampe rampe_alpha = Rampe();

double output4Delta, output4Alpha;
double currentDelta, currentAlpha;
double consigneDelta, consigneAlpha;
PID pid4DeltaControl(KP_DELTA,KI_DELTA,KD_DELTA);
PID pid4AlphaControl(KP_ALPHA,KI_ALPHA,KD_ALPHA);


Robot::Robot(long *value_left_enc, long *value_right_enc, AF_DCMotor * motor_left, AF_DCMotor * motor_right)
{
	_value_left_enc = value_left_enc;
	_value_right_enc = value_right_enc;
	_prev_value_left_enc = (*_value_left_enc);
	_prev_value_right_enc = (*_value_right_enc);

	_motor_left = motor_left;
	_motor_right = motor_right;

	_rampe_delta = &rampe_delta;
	_rampe_alpha = &rampe_alpha;
}

void Robot::init()
{
	_x = 0;
	_y = 0;
	_a = 0.0;
	_speed = 0.0;
	
	_goal_reached = true;
	
	Robot::cancel();
}

void Robot::setGoal(T_GOAL t, long int x, long int y, double a)
{
	_goal.type = t;
	_goal.x = x;
	_goal.y = y;
	_goal.a = a;
}
	
bool Robot::goalIsReached()
{
	return _goal_reached;
}

void Robot::update(int dt)
{
	Robot::update_state(dt);
	Robot::update_motors(dt);
}

void Robot::update_state(int dt)
{
	long int left_enc = (*_value_left_enc);
	long int right_enc = (*_value_right_enc);
	
	/*calcul du deplacement depuis la derniere fois en ticks */
	long dl = left_enc - _prev_value_left_enc;
	long dr = right_enc - _prev_value_right_enc;

	/*preparation de la prochaine iteration*/
	_prev_value_left_enc = left_enc;
	_prev_value_right_enc = right_enc;

	/* calcul de la vitesse en mm/s
	 * estimation : simple moyenne */
	double speed_left = ((double)dl/(double)dt)*1000.0;
	double speed_right = ((double)dr/(double)dt)*1000.0;
	double speed = (speed_left+speed_right)/2.0;

	/* calcul du changement d'orientation en rad */
	double delta_angle = (double)(dr-dl)/(double)ENC_CENTER_DIST_TICKS;

	/* calcul du deplacement */
	double delta_dist = (double)(dr+dl)/2.0;

	/* mise a jour de la position en ticks
	 * on utilise des cos et des sin et c'est pas tres opti.
	 * A voir si l'approximation par un dev limite d'ordre 2 est plus efficace
	 */
	double dx = delta_dist*cos(_a+delta_angle);
	double dy = delta_dist*sin(_a+delta_angle);

	/*mise a jour de l'etat du robot  */
	_speed = speed;
	_a = moduloPI(_a + delta_angle);
	_x += dx;
	_y += dy;
}

void Robot::reset_pid()
{
	pid4DeltaControl.reset();
	pid4DeltaControl.setOutputLimits(-255,255);
	consigneDelta = 0.0;
	
	pid4AlphaControl.reset();
	pid4AlphaControl.setOutputLimits(-255,255);
	consigneAlpha = 0.0;
}


void Robot::update_motors(int dt)
{
	static long int _i = 0;
	
	_rampe_alpha->compute_next_goal(dt);
	_rampe_delta->compute_next_goal(dt);
	
	double angleDiffAlpha = angle_diff(_goal.a, _a);
	double angleDiffDelta = angle_diff(atan2(_goal.y-_y,_goal.x-_x), _a);
	
	double angleDiff = 0.0;
	switch (_goal.type)
	{
		case G_POS:
			angleDiff = angleDiffDelta;
		break;

		case G_ANG:
			angleDiff = angleDiffAlpha;
		break;
	}

	int sens_delta=-1;
	// Si le goal est derrière 
	if(_rampe_delta->get_phase() == PHASE_END and abs(angleDiffDelta) > M_PI/2)
	{
		sens_delta = -sens_delta;
		if (_goal.type == G_POS)
		{
			angleDiff = moduloPI(M_PI + angleDiff);
		}
	}
	
 	double dx = _goal.x-_x;
	double dy = _goal.y-_y;

	
	currentDelta = sens_delta * abs(_rampe_delta->get_goal() - sqrt(dx*dx+dy*dy));
	currentAlpha = _rampe_alpha->get_goal() - (angleDiff * ENC_CENTER_DIST_TICKS);
	

	int value_pwm_left = 0;
	int value_pwm_right = 0;
	if ((G_POS == _goal.type and abs(currentDelta) < 10*ENC_MM_TO_TICKS)
		or (G_ANG == _goal.type and abs(angleDiff) < (3.0f/180.0f*M_PI)))
	{
		_goal_reached = true;
	}
	else
	{
		if (G_POS == _goal.type)
			output4Delta = pid4DeltaControl.compute(currentDelta);
		else
			output4Delta = 0;
		output4Alpha = pid4AlphaControl.compute(currentAlpha);
		
		value_pwm_left = output4Delta-output4Alpha;
		value_pwm_right = output4Delta+output4Alpha;
		
		// Débordement
		if (value_pwm_right > 255)
			value_pwm_right = 255;
		else if (value_pwm_right < -255)
			value_pwm_right = -255;
		
		if (value_pwm_left > 255)
			value_pwm_left = 255;
		else if (value_pwm_left < -255)
			value_pwm_left = -255;
			
		/*
		if (micros() - _i > 1000000) {
			Serial.println("update_motors_delta");
			Serial.println(output4Delta);
			Serial.println(currentDelta);
			Serial.print("goal");
			Serial.println(_rampe_delta->get_goal());
			Serial.print("diff");
			Serial.println(sens_delta * sqrt(dx*dx+dy*dy));
			Serial.print("sens");
			Serial.println(sens_delta);
			Serial.println("update_motors_alpha");
			Serial.println(output4Alpha);
			Serial.println(currentAlpha);
			Serial.print("goal");
			Serial.println(_rampe_alpha->get_goal());
			Serial.print("diff");
			Serial.println(angle_diff(goal_a,_a) * 180.0f / M_PI);
			_i = micros();
		}//*/
		_goal_reached = false;
	}

	setLeftPWM(-value_pwm_left);
	setRightPWM(-value_pwm_right);
	
	
}

void Robot::go_to(long int x, long int y, double speed)
{
	
	_goal.type = G_POS;
	_goal.x = x;
	_goal.y = y;
	_goal.a = 0;
	Robot::reset_pid();

	double a = atan2(y, x);
	_rampe_alpha->compute(angle_diff(a,_a) * ENC_CENTER_DIST_TICKS, 0, speed, 0.1, -0.025);

 	double dx = x-_x;
	double dy = y-_y;
	double d = (double)sqrt(dx*dx+dy*dy);
	_rampe_delta->compute(d, 0, speed, 0.1, -0.05);
}

void Robot::turn(double a, double speed)
{
	_goal.type = G_ANG;
	_goal.x = _x;
	_goal.y = _y;
	_goal.a = a;
	Robot::reset_pid();
	
	_rampe_alpha->compute(angle_diff(a,_a) * ENC_CENTER_DIST_TICKS, 0, speed, 0.1, -0.025);
	_rampe_delta->compute(0, 0, 1, 1, -1);
}


void Robot::cancel()
{
	Robot::setGoal(G_ANG,_x,_y,_a);
	
	Robot::reset_pid();
	_rampe_alpha->compute(0, 0, 1, 1, -1);
	_rampe_delta->compute(0, 0, 1, 1, -1);
}


int Robot::get_x()
{
	return _x;
}

int Robot::get_y()
{
	return _y;
}

double Robot::get_a()
{
	return _a;
}






