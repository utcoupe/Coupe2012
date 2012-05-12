#ifndef GLOBALS_H
#define GLOBALS_H

#include "observer.h"
#include "encoder.h"
#include "regulator.h"

extern int g_debug_on;
extern RobotObserver g_observer;
//extern Encoder g_left_encoder;
//extern Encoder g_right_encoder;


extern PosAndSpeedRegulator g_delta_regulator;
extern PosAndSpeedRegulator g_alpha_regulator;
extern bool g_stop;

extern double G_angle;

#endif


