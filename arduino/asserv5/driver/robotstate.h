/*
 * robotstate.h
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */

typedef struct RobotState {
  long int x;
  long int y;
  double speed;
  double a;
} RobotState;


extern int value_pwm_left;
extern int value_pwm_right;

void initRobotState();


void robot_set_x(long int x);
void robot_set_mm_x(long int x);
void robot_set_ticks_x(long int x);

void robot_set_y(long int y);
void robot_set_mm_y(long int y);
void robot_set_ticks_y(long int y);

void robot_set_rad_angle(double a);
void robot_set_deg_angle(double a);

double robot_get_angle();
long int robot_get_x();
long int robot_get_y();


void computeRobotState();
double moduloPI(double Nb);

