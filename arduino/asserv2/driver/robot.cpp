#include "robot.h"
#include "encoder.h"
#include "PID_Beta6.h"
#include "tools.h"

#include <math.h>



double angle_diff(double a, double b)
{
	return atan2(sin(a-b), cos(a-b));
}
	


Robot robot(&value_left_enc, &value_right_enc, &moteurG, &moteurD);

Rampe rampe_delta = Rampe();
Rampe rampe_alpha = Rampe();

double output4Delta, output4Alpha;
double currentDelta, currentAlpha;
double consigneDelta, consigneAlpha;
PID pid4DeltaControl(&currentDelta,&output4Delta,&consigneDelta,KP_DELTA,KI_DELTA,KD_DELTA);
PID pid4AlphaControl(&currentAlpha,&output4Alpha,&consigneAlpha,KP_ALPHA,KI_ALPHA,KD_ALPHA);


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

	_x = 0;
	_y = 0;
	_a = 0.0;
	_speed = 0.0;
}

void Robot::update(int dt)
{
	Robot::update_state(dt);
	Robot::update_motors(dt);
}

void Robot::update_state(int dt)
{
	/*calcul du deplacement depuis la derniere fois en ticks */
	long dl = (*_value_left_enc) - _prev_value_left_enc;
	long dr = (*_value_right_enc) - _prev_value_right_enc;

	/*preparation de la prochaine iteration*/
	_prev_value_left_enc = (*_value_left_enc);
	_prev_value_right_enc = (*_value_right_enc);

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
	_a = _a + delta_angle;
	_x += dx;
	_y += dy;
}

void Robot::reset_pid()
{
	pid4DeltaControl.Reset();
	pid4DeltaControl.SetInputLimits(-TABLE_DISTANCE_MAX_MM/ENC_TICKS_TO_MM,TABLE_DISTANCE_MAX_MM/ENC_TICKS_TO_MM);
	pid4DeltaControl.SetSampleTime(DUREE_CYCLE);
	pid4DeltaControl.SetOutputLimits(-255,255); /*composante liee a la vitesse lineaire*/
	pid4DeltaControl.SetMode(AUTO);
	pid4AlphaControl.Reset();
	pid4AlphaControl.SetSampleTime(DUREE_CYCLE);
	//pid4AlphaControl.SetInputLimits(-M_PI,M_PI);
	pid4AlphaControl.SetOutputLimits(-255,255); /*composante lie a la vitesse de rotation*/
	pid4AlphaControl.SetMode(AUTO);
}


void Robot::update_motors(int dt)
{
	_rampe_alpha->compute_next_goal(dt);
	_rampe_delta->compute_next_goal(dt);


	switch (_goal.type)
	{
		case G_POS:
			currentAlpha = fmod(atan2(_goal.y-_y,_goal.x-_x) - _a, M_PI);
		break;

		case G_ANG:
			currentAlpha = fmod(_goal.a - _a, M_PI);
		break;
	}

	int sens=1;
	// Si le goal est derrière 
	if(_rampe_delta->get_phase() == PHASE_END and abs(currentAlpha) > M_PI/2)
	{
		sens = -1;
	}
	
 	double dx = _goal.x-_x;
	double dy = _goal.y-_y;
	currentDelta = sens * sqrt(dx*dx+dy*dy);
	
	consigneAlpha = _rampe_alpha->get_goal() - currentAlpha;
	consigneDelta = _rampe_delta->get_goal() - currentDelta;

	
	pid4DeltaControl.Compute();
	pid4AlphaControl.Compute();

	int value_pwm_left = 0;
	int value_pwm_right = 0;
	if ((G_POS == _goal.type and abs(currentDelta) > 5*ENC_MM_TO_TICKS)
		or (G_ANG == _goal.type and abs(currentAlpha) > 3.0f*M_PI/360.0f))
	{
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
	}

	setLeftPWM(value_pwm_left);
	setRightPWM(value_pwm_right);
}


void Robot::go_to(long int x, long int y, long int speed)
{
	_goal.type = G_POS;
	_goal.x = x;
	_goal.y = y;
	_goal.a = 0;
	Robot::reset_pid();

	double a = atan2(y, x);
	_rampe_alpha->compute((a-_a)*ENC_CENTER_DIST_TICKS, 0, speed, 2000, -500);

 	long dx = x-_x;
	long dy = y-_y;
	long d = (long)sqrt(dx*dx+dy*dy);
	_rampe_delta->compute(d, 0, speed, 2000, -500);
}

void Robot::turn(double a, long int speed)
{
	_goal.type = G_ANG;
	_goal.x = _x;
	_goal.y = _y;
	_goal.a = a;
	Robot::reset_pid();
	
	_rampe_alpha->compute((long int)(angle_diff(a,_a) * ENC_CENTER_DIST_TICKS), 0, speed, 2000, -500);
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






