#ifndef COMMAND_H_
#define COMMAND_H_


#include <inttypes.h>

void cmd(int id, int id_cmd, int * args, int size);


extern double G_consign;
extern int G_debug_on;


#include "observer.h"
#include "regulator.h"
extern PositionObserver observerG, observerD;
extern PosAndSpeedRegulator regulatorG, regulatorD;


#endif /* COMMAND_H_ */


