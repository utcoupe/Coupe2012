#ifndef GLOBALS_H
#define GLOBALS_H

#include "observer.h"
#include "encoder.h"
#include "regulator.h"

extern int g_debug_on;
extern RobotObserver g_observer;
//extern Encoder g_left_encoder;
//extern Encoder g_right_encoder;

typedef enum {GOAL_POS, GOAL_ANG} GOAL_T;
extern GOAL_T g_goal_type;
extern int g_goal_x;
extern int g_goal_y;
extern double g_goal_a;


extern PosAndSpeedRegulator g_delta_regulator;
extern PosAndSpeedRegulator g_alpha_regulator;


#endif


