#ifndef _ROBOT_H
#define _ROBOT_H

#include <math.h>

#include "parameters.h"
#include "motor.h"
#include "rampe.h"


long int mm_to_robotunit(int x);
int degrees_to_robotunit(double a);

typedef enum {
	G_POS,
	G_ANG,
	G_PWM
} T_GOAL;

typedef struct {
	T_GOAL type;
	long int x;
	long int y;
	double a;
	double speed;
	int pwm;
} Goal;

class Robot
{
	public:
		Robot(long *value_enc_left, long *value_enc_right, AF_DCMotor * motor_left, AF_DCMotor * motor_right);
		void init();
		void update(int dt);
		void reset_pid();
		void go_to(long int x, long int y, double speed, double final_speed=0);
		void set_pwm_right_left(int pwm);
		void turn(double a, double speed = 200);
		void update_speedf(double speed);
		void cancel();
		void set_x_y_a(long int x, long int y, double a);
		void set_coeffs_pid_position_d(double p, double i, double d);
		void set_coeffs_pid_position_a(double p, double i, double d);
		void set_coeffs_pid_angle_d(double p, double i, double d);
		void set_coeffs_pid_angle_a(double p, double i, double d);
		bool goalIsReached();
		
		Goal get_goal();
		
		/* mm */
		int get_x();
		/* mm */
		int get_y();
		/* rad */
		double get_a();

		
	private:
		void update_state(int dt);
		void update_motors(int dt);
		void setGoal(T_GOAL t, long int x, long int y, double a, double speed, int pwm);
		void setGoalPwm(int pwm);
		void setGoalPosition(long int x, long int y, double speed);
		void setGoalAngle(double a, double speed);

		/* ticks */
		long int x, y;
		/* rad */
		double a;
		/* ticks/µs */
		double speed;
		/* ticks */
		long int *value_left_enc, *value_right_enc;
		/* ticks */
		long int prev_value_left_enc, prev_value_right_enc;
		
		AF_DCMotor *motor_left, *motor_right;
		
		Rampe *rampe_delta, *rampe_alpha;
		Goal goal;

		bool goal_reached;
};

extern Robot robot;


#endif


