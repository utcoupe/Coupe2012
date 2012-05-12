/*
 * parameters.h
 *
 *  Created on: 16 janv. 2011
 *      Author: HoHen
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "math.h"

// Duree d'un cycle (en millisecondes)
#define DUREE_CYCLE 2


/* Parametre de la table en mm*/
#define TABLE_WIDTH_MM 2100.0
#define TABLE_HEIGHT_MM 3000.0
#define TABLE_DISTANCE_MAX_MM 3661.96

/*Parametre de l'asservissement*/
#define KP_SPEED 2.0
#define KI_SPEED 5.0
#define KD_SPEED 1.0
#define KP_ANGLE 14.0
#define KI_ANGLE 1.5
#define KD_ANGLE 20000.0
#define KP_DELTA 14.0
#define KI_DELTA 1.5
#define KD_DELTA 5000.0
#define KP_ALPHA 5.0
#define KI_ALPHA 1.5
#define KD_ALPHA 5000.0

/*Parametre de la file des taches a effectuer*/
#define SIZE 15


#endif /* PARAMETERS_H_ */
