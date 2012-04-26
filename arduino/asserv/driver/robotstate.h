/*
 * robotstate.h
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */

typedef struct RobotState {
  double x;
  double y;
  double speed;
  double angle;
} RobotState;

extern RobotState robot_state;

extern int value_pwm_left;
extern int value_pwm_right;

void initRobotState();
