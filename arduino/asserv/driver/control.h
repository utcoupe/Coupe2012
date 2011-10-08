/*
 * control.h
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <math.h>
#include "parameters.h"

#define PHASE_1 1
#define PHASE_2 2
#define PHASE_3 3
#define PHASE_4 4
#define PHASE_5 5

typedef struct CurrentGoal {
	int type;
	int id;
	double speed;
	double period;
	double x;
	double y;
	double angle;
	bool isReached;
	bool isCanceled;
	bool isPaused;
	bool isMessageSent;
	int phase;
} CurrentGoal;

extern CurrentGoal current_goal;

void initController();

void speedControl(int*,int*);
void angleControl(int*,int*);
void positionControlCurviligne(int*,int*);
void positionControl(int*,int*);
void delayControl(int*, int*);
void pwmControl(int*, int*);

void computeRobotState();

double moduloPI(double Nb);

#endif /* CONTROL_H_ */
