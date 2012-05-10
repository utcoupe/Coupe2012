/*
 * control.cpp
 *
 *  Created on: 13 janv. 2011
 *	  Author: HoHen
 */


/* Quelques trucs sur le PID
 *
 * kp : le terme proportionnel qui permet d'augmenter la vitesse de montee (atteint la consigne le plus rapidement possible).
 * ki : le terme integral qui reduit l'erreur statique.
 * kd : le terme derive qui reduit le depassement (l'overshoot).
 * __________________________________________________________________________________________________
 * Coefficient		|Temps de montee	|Temps de stabilisation	|Depassement	|Erreur statique	|
 *	kp				|Diminue			|Augmente				|Augmente		|Diminue			|
 *	ki				|Diminue			|Augmente				|Augmente		|Annule				|
 *	kd				|-					|Diminue				|Diminue		|-					|
 * _________________|___________________|_______________________|_______________|___________________|
 */

#include "include_arduino.h"


#include "control.h"
#include "encoder.h"
#include "robotstate.h"
#include "PID_Beta6.h"
#include "message.h"


CurrentGoal current_goal;

void initController(){
	current_goal.isReached = true;
	current_goal.isCanceled = false;
	current_goal.isMessageSent = false;
	current_goal.isPaused = false;
}






