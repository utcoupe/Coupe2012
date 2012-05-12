#include "globals.h"

int g_debug_on(0);
//Encoder g_left_encoder;
//Encoder g_right_encoder;
RobotObserver g_observer;


PosAndSpeedRegulator g_delta_regulator;
PosAndSpeedRegulator g_alpha_regulator;


bool g_stop = false;
