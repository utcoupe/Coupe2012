#ifndef _ROBOT_H
#define _ROBOT_H

#include <math.h>

#include "parameters.h"
#include "motor.h"
#include "rampe.h"


typedef enum {
	G_POS,
	G_ANG
} T_GOAL;

typedef struct {
	T_GOAL type;
	int x;
	int y;
	double a;
} Goal;

class Robot
{
	public:
		Robot(long *value_enc_left, long *value_enc_right, AF_DCMotor * motor_left, AF_DCMotor * motor_right);
		void update(int dt);
		void reset_pid();
		void go_to(int x, int y, double speed = 200);
		void turn(double a, double speed = 200);

		/* mm */
		int get_x();
		/* mm */
		int get_y();
		/* rad */
		double get_a();
		
	private:
		void update_state(int dt);
		void update_motors(int dt);
		
		long int _x, _y;
		double _a;
		double _speed;
		long *_value_left_enc, *_value_right_enc;
		long _prev_value_left_enc, _prev_value_right_enc;
		AF_DCMotor *_motor_left, *_motor_right;
		Rampe *_rampe_delta, *_rampe_alpha;
		Goal _goal;
};

extern Robot robot;


#endif


