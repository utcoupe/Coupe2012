/*
 * robotstate.cpp
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */

#include "robotstate.h"
#include "parameters.h"
#include "math.h"

int value_pwm_left;
int value_pwm_right;
RobotState robot_state;

void initRobotState(){
	robot_state.x = 0;
	robot_state.y = 0;
	robot_state.speed = 0;
	robot_state.a = 0;
}

void robot_set_x(long int x) {
	robot_state.x = x;
}

void robot_set_mm_x(long int x) {
	robot_set_ticks_x(x*ENC_MM_TO_TICKS);
}

void robot_set_ticks_x(long int x) {
	robot_state.x = x*100;
}

void robot_set_y(long int y) {
	robot_state.y = y;
}

void robot_set_mm_y(long int y) {
	robot_set_ticks_y(y*ENC_MM_TO_TICKS);
}

void robot_set_ticks_y(long int y) {
	robot_state.y = y*100;
}

void robot_set_rad_angle(double a) {
	robot_state.a = a;
}

void robot_set_deg_angle(double a) {
	robot_state.a = a*2.0*M_PI/180.0;
}

double robot_get_angle() {
	return robot_state.a;
}

long int robot_get_x() {
	return robot_state.x;
}

long int robot_get_y() {
	return robot_state.y;
}

