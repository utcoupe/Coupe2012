/*
 * fifo.h
 *
 *  Created on: 16 janv. 2011
 *	  Author: HoHen
 */

#ifndef FIFO_H_
#define FIFO_H_

#include "parameters.h"
#include "control.h"

/*Constantes pour la liste des taches*/
#define NO_ID -1

#define TYPE_SPEED 0
#define TYPE_ANGLE 1
#define TYPE_POSITION 2
#define TYPE_CALIB_X 3
#define TYPE_CALIB_Y 4
#define TYPE_CALIB_ANGLE 5
#define TYPE_DELAY 6
#define TYPE_PWM 7


typedef struct {
	int type; /*1,2,3 selon le type d'asserv*/
	int id; /* identifiant de la demande, utile pour envoyer une confirmation a la strategie, -1 si pas besoin de message */
	double data_1; /*speed (pour but en vitesse) ou x ou angle*/
	double data_2; /* y ou speed (pour but en angle) ou period*/
	double data_3; /* speed (pour but en position)*/
} Goal;

typedef struct {
	Goal* goal;
	int in;
	int out;
} Fifo;


void initGoals();

void pushGoalPosition(int,double,double,double);
void pushGoalOrientation(int,double,double);
void pushGoalSpeed(int,double,double);
void pushGoalAutoCalibration(int,bool);
void pushGoalManualCalibration(int,double);
void pushGoalDelay(double);
void pushGoalPwm(int,double,double);

void popGoal();
void clearGoals();
bool fifoIsEmpty();

#endif /* FIFO_H_ */
