#ifndef CONTROL_H
#define CONTROL_H

#include "regulator.h"

void positionControl(int32_t goal_x, int32_t goal_y, double goal_a,
	int32_t current_x, int32_t current_y, double current_a, int32_t current_speed, double current_speed_a,
	int& pwm_left, int& pwm_right, bool & reached);
void angleControl(int32_t goal_x, int32_t goal_y, double goal_a,
	int32_t current_x, int32_t current_y, double current_a, int32_t current_speed, double current_speed_a,
	int& pwm_left, int& pwm_right, bool & reached);

void alphaSpeedControl(double goal_speed, double current_speed, PosAndSpeedRegulator& regul,
	int& pwm_left, int& pwm_right);
void deltaSpeedControl(double goal_speed, double current_speed, PosAndSpeedRegulator& regul,
	int& pwm_left, int& pwm_right);



void dump();


#endif


