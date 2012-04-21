#include "globals.h"

int g_debug_on(0);
//Encoder g_left_encoder;
//Encoder g_right_encoder;
RobotObserver g_observer;

GOAL_T g_goal_type = GOAL_POS;
int g_goal_x=0;
int g_goal_y=0;
double g_goal_a=0.0;

PosAndSpeedRegulator g_delta_regulator;
PosAndSpeedRegulator g_alpha_regulator;
