#include "robot.h"
#include "encoder.h"
#include "WProgram.h"
#include "wiring.h"


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

long int mm_to_robotunit(int x)
{
	return x * ENC_MM_TO_TICKS;
}

int degrees_to_robotunit(double a)
{
	return a * DEG_TO_RAD;
}


Robot robot(&value_left_enc, &value_right_enc, &moteurG, &moteurD);

Rampe __rampe_delta = Rampe();
Rampe __rampe_alpha = Rampe();


PID pid4delta_angle_control(KP_D_ANGLE, KI_D_ANGLE, KD_D_ANGLE);
PID pid4alpha_angle_control(KP_A_ANGLE, KI_A_ANGLE, KD_A_ANGLE);
PID pid4delta_position_control(KP_D_POSITION, KI_D_POSITION, KD_D_POSITION);
PID pid4alpha_position_control(KP_A_POSITION, KI_A_POSITION, KD_A_POSITION);

Robot::Robot(long *value_left_enc, long *value_right_enc, AF_DCMotor * motor_left, AF_DCMotor * motor_right)
{
	this->value_left_enc = value_left_enc;
	this->value_right_enc = value_right_enc;
	this->prev_value_left_enc = *(value_left_enc);
	this->prev_value_right_enc = *(value_right_enc);

	this->motor_left = motor_left;
	this->motor_right = motor_right;

	this->rampe_delta = &__rampe_delta;
	this->rampe_alpha = &__rampe_alpha;
}

void Robot::init()
{
	this->x = 0;
	this->y = 0;
	this->a = 0.0;
	this->speed = 0.0;
	
	this->goal_reached = true;
	
	Robot::cancel();
	
	Robot::reset_pid();
}

void Robot::setGoal(T_GOAL t, long int x, long int y, double a, double speed, int pwm)
{
	this->goal.type = t;
	this->goal.x = x;
	this->goal.y = y;
	this->goal.a = a;
	this->goal.speed = speed;
	this->goal.pwm = pwm;
}

void Robot::setGoalPosition(long int x, long int y, double speed)
{
	Robot::setGoal(G_POS, x, y, 0, speed, 0);
}

void Robot::setGoalAngle(double a, double speed)
{
	Robot::setGoal(G_ANG, this->x, this->y, a, speed, 0);
}

void Robot::setGoalPwm(int pwm)
{
	Robot::setGoal(G_PWM, 0, 0, 0, 0, pwm);
}
	
bool Robot::goalIsReached()
{
	return this->goal_reached;
}

void Robot::reset_pid()
{
	pid4delta_angle_control.reset().setOutputLimits(-255,255);
	pid4alpha_angle_control.reset().setOutputLimits(-255,255);
	pid4delta_position_control.reset().setOutputLimits(-255,255);
	pid4alpha_position_control.reset().setOutputLimits(-255,255);
}

void Robot::update(int dt)
{
	Robot::update_state(dt);
	Robot::update_motors(dt);
}

void Robot::update_state(int dt)
{
	long int left_enc = *(this->value_left_enc);
	long int right_enc = *(this->value_right_enc);
	
	/* calcul du deplacement depuis la derniere fois en ticks */
	long dl = left_enc - this->prev_value_left_enc;
	long dr = right_enc - this->prev_value_right_enc;

	/* preparation de la prochaine iteration */
	this->prev_value_left_enc = left_enc;
	this->prev_value_right_enc = right_enc;

	/*
	 * calcul de la vitesse en mm/s
	 * estimation : simple moyenne
	 */
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
	double dx = delta_dist*cos(this->a+delta_angle);
	double dy = delta_dist*sin(this->a+delta_angle);

	/*mise a jour de l'etat du robot  */
	this->speed = speed;
	this->a = moduloPI(this->a + delta_angle);
	this->x += dx;
	this->y += dy;
}


void Robot::update_motors(int dt)
{
	static PHASE current_phase_alpha = PHASE_END;
	static PHASE current_phase_delta = PHASE_END;

	int value_pwm_left = 0;
	int value_pwm_right = 0;
	
	if (this->goal.type == G_PWM)
	{
		value_pwm_left = goal.pwm;
		value_pwm_right = goal.pwm;
	}
	else
	{
		this->rampe_alpha->compute_next_goal(dt);
		this->rampe_delta->compute_next_goal(dt);
		
		double angleDiffAlpha = angle_diff(this->goal.a, this->a);
		double angleDiffDelta = angle_diff(atan2(this->goal.y-this->y, this->goal.x-this->x), this->a);
		
		double angleDiff = 0.0;
		switch (this->goal.type)
		{
			case G_POS:
				angleDiff = angleDiffDelta;
			break;

			case G_ANG:
				angleDiff = angleDiffAlpha;
			break;
		}

		int sens_delta=1;
		// Si le goal est derrière
		if(abs(angleDiffDelta) > M_PI/2)
		{
			sens_delta = -sens_delta;
			if (this->goal.type == G_POS)
			{
				angleDiff = moduloPI(M_PI + angleDiff);
			}
		}
		
		double dx = this->goal.x-this->x;
		double dy = this->goal.y-this->y;

		double deltaDiff = sqrt(dx*dx+dy*dy);
		double currentDelta = this->rampe_delta->get_pos() - sens_delta * deltaDiff;
		double currentAlpha = this->rampe_alpha->get_pos() - (angleDiff * ENC_CENTER_DIST_TICKS);

		if (this->rampe_delta->get_phase() != current_phase_delta)
		{
			Serial.println("DELTA");
			switch(this->rampe_delta->get_phase())
			{
				case PHASE_ACCEL:
				Serial.println("accel");
				break;
				case PHASE_CONST:
				Serial.println("const");
				break;
				case PHASE_DECEL:
				Serial.println("decel");
				break;
				case PHASE_END:
				Serial.println("end");
				break;
				default:
				Serial.println("euh...");
				break;
			}
			Serial.println(deltaDiff);
			Serial.println(this->rampe_delta->get_pos());
			current_phase_delta = this->rampe_delta->get_phase();
		}
		if (this->rampe_alpha->get_phase() != current_phase_alpha)
		{
			Serial.println("ALPHA");
			switch(this->rampe_alpha->get_phase())
			{
				case PHASE_ACCEL:
				Serial.println("accel");
				break;
				case PHASE_CONST:
				Serial.println("const");
				break;
				case PHASE_DECEL:
				Serial.println("decel");
				break;
				case PHASE_END:
				Serial.println("end");
				break;
				default:
				Serial.println("euh...");
				break;
			}
			Serial.println((angleDiff * ENC_CENTER_DIST_TICKS));
			Serial.println(this->rampe_alpha->get_pos());
			current_phase_alpha = this->rampe_alpha->get_phase();
		}
		

		if (this->rampe_alpha->get_phase() == PHASE_END and this->rampe_delta->get_phase() == PHASE_END
			and deltaDiff < 10*ENC_MM_TO_TICKS
			and (this->goal.type == G_POS or abs(angleDiff) < (3.0f/180.0f*M_PI)))
		{
			this->goal_reached = true;
		}
		else
		{
			double output4Delta=0,output4Alpha=0;
			if (this->goal.type == G_POS)
			{
				output4Delta = pid4delta_position_control.compute(currentDelta);
				output4Alpha = pid4alpha_position_control.compute(currentAlpha);
			}
			else
			{
				output4Delta = pid4delta_angle_control.compute(currentDelta);
				output4Alpha = pid4alpha_angle_control.compute(currentAlpha);
			}
			
			value_pwm_left = output4Delta-output4Alpha;
			value_pwm_right = output4Delta+output4Alpha;
			
			// Débordement
			if (value_pwm_right > 255)
			{
				value_pwm_left = value_pwm_left * 255 / value_pwm_right;
				value_pwm_right = 255;
			}
			else if (value_pwm_left > 255)
			{
				value_pwm_left = value_pwm_right * 255 / value_pwm_left;
				value_pwm_left = 255;
			}
			else if (value_pwm_right < -255)
			{
				value_pwm_left = value_pwm_left * -255 / value_pwm_right;
				value_pwm_right = -255;
			}
			else if (value_pwm_left < -255)
			{
				value_pwm_right = value_pwm_right * -255 / value_pwm_left;
				value_pwm_left = -255;
			}/*
			if (value_pwm_right > 255)
				value_pwm_right = 255;
			else if (value_pwm_right < -255)
				value_pwm_right = -255;
			
			if (value_pwm_left > 255)
				value_pwm_left = 255;
			else if (value_pwm_left < -255)
				value_pwm_left = -255;*/
			
			this->goal_reached = false;
		}
	}

	setLeftPWM(-value_pwm_left);
	setRightPWM(-value_pwm_right);
	
	
}

void Robot::go_to(long int x, long int y, double goal_speed, double final_speed)
{
	#define DEBUG_GOTO
	#ifdef DEBUG_GOTO
	Serial.println(__PRETTY_FUNCTION__);
	Serial.print("x "); Serial.println(x);
	Serial.print("y "); Serial.println(y);
	Serial.print("speed "); Serial.println(speed);
	Serial.print("final_speed "); Serial.println(final_speed);
	#endif
	
	Robot::setGoalPosition(x,y,speed);

	double a = atan2(y, x);
	this->rampe_alpha->compute(angle_diff(a, this->a) * ENC_CENTER_DIST_TICKS, 0, goal_speed, 0.05, -0.05, 0, 0);

 	double dx = x-this->x;
	double dy = y-this->y;
	double d = (double)sqrt(dx*dx+dy*dy);
	this->rampe_delta->compute(d, 0, goal_speed, 0.01, -0.01, this->speed, final_speed);

	
	this->goal_reached = false;
}

void Robot::turn(double a, double goal_speed)
{
	Robot::setGoalAngle(a,speed);
	
	this->rampe_alpha->compute(angle_diff(a, this->a) * ENC_CENTER_DIST_TICKS, 0, speed, 0.05, -0.05, 0,0);
	this->rampe_delta->compute(0, 0, 1, 0.05, -0.05, 0,0);
	
	this->goal_reached = false;
}

void Robot::set_pwm_right_left(int pwm)
{
	Robot::setGoalPwm(pwm);
}


void Robot::cancel()
{
	Robot::setGoalPosition(this->x, this->y, 1);
	
	this->rampe_alpha->compute(0, 0, 1, 1, -1,0,0);
	this->rampe_delta->compute(0, 0, 1, 1, -1,0,0);
}


int Robot::get_x()
{
	return this->x;
}

int Robot::get_y()
{
	return this->y;
}

double Robot::get_a()
{
	return this->a;
}

Goal Robot::get_goal()
{
	return this->goal;
}

void Robot::update_speedf(double speedf)
{
	Serial.println("update_speedf");
	Robot::go_to(this->goal.x, this->goal.y, this->goal.speed, speedf);
}

void Robot::set_x_y_a(long int x, long int y, double a)
{
	this->x = x;
	this->y = y;
	this->a = a;
}

void Robot::set_coeffs_pid_position_d(double p, double i, double d)
{
	pid4delta_position_control.setCoeffs(p,i,d);
}

void Robot::set_coeffs_pid_position_a(double p, double i, double d)
{
	pid4alpha_position_control.setCoeffs(p,i,d);
}

void Robot::set_coeffs_pid_angle_d(double p, double i, double d)
{
	pid4delta_angle_control.setCoeffs(p,i,d);
}

void Robot::set_coeffs_pid_angle_a(double p, double i, double d)
{
	pid4alpha_angle_control.setCoeffs(p,i,d);
}


