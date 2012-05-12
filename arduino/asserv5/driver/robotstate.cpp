/*
 * robotstate.cpp
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */

#include "robotstate.h"
#include "parameters.h"
#include "math.h"
#include "encoder.h"

int value_pwm_left;
int value_pwm_right;
RobotState robot_state;
double a0=0.0;

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
	a0 = a-(double)(value_right_enc-value_left_enc)/(double)ENC_CENTER_DIST_TICKS;
	robot_state.a = a;
}

void robot_set_deg_angle(double a) {
	a0 = a*M_PI/180.0-(double)(value_right_enc-value_left_enc)/(double)ENC_CENTER_DIST_TICKS;
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


/* Implementation du modele d'evolution du robot a partir de l'odometrie
 * A appeler a intervalle regulier (a voir pour la mettre sur une interruption timer)
 * */
void computeRobotState(){
	static long int prev_value_left_enc = 0;
	static long int prev_value_right_enc = 0;

	/*calcul du deplacement depuis la derniere fois en ticks */
	long int dl = value_left_enc - prev_value_left_enc;
	long int dr = value_right_enc - prev_value_right_enc;

	/*preparation de la prochaine iteration*/
	prev_value_left_enc = value_left_enc;
	prev_value_right_enc = value_right_enc;

	/* calcul du deplacement */
	long int delta_dist = (dr+dl)*50;

	/*mise a jour de l'etat du robot  */
	//robot_state.speed = 0;
	robot_state.a = moduloPI(a0+(double)(value_right_enc-value_left_enc)/(double)ENC_CENTER_DIST_TICKS);
	robot_set_x(robot_state.x + (double)delta_dist*cos(robot_state.a));
	robot_set_y(robot_state.y + (double)delta_dist*sin(robot_state.a));
}


/* Fonction de calcul du modulo PI ]-PI,PI]
 * moduloPi( 3*PI/2 )= - PI/2
 * moduloPi( PI ) = PI
 * moduloPi( 0 ) = 0
 *
 * x-y*((int)(x/y)) => c'est le modulo pour des doubles (x%y)
 * trunc et int c'est pareil au final, j'ai choisi trunc juste pour que ce soit plus complique !
 * */
double moduloPI(double Nb){
	double result;
	if (Nb > M_PI)
		result = Nb - (2*M_PI)*trunc((Nb + M_PI) / (2*M_PI));
	else  if (Nb <= -M_PI)
		result = Nb - (2*M_PI)*trunc((Nb - M_PI) / (2*M_PI));
	else result = Nb;
	return(result);
}




