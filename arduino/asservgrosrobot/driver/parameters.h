/*
 * parameters.h
 *
 *  Created on: 16 janv. 2011
 *      Author: HoHen
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "math.h"

/* Parametres mecanique du robot */
#define DIST_MOTOR_AXIS_TO_BACK_MM 122.0 // Distance entre le l'axe des moteurs et l'arriere du robot


const int ENC_RESOLUTION = 500; // Nombre de points par tour d'encodeur
const int ENC_RADIUS = 34; // Rayon de la roue codeuse

const double ENC_TICKS_TO_MM = (2.0*M_PI*((double)ENC_RADIUS))/(((double)ENC_RESOLUTION)*4.0); // en mm.tick^-1 (opti: a calculer apres) environ : 0.0565
const double ENC_MM_TO_TICKS = (((double)ENC_RESOLUTION)*4.0)/(2.0*M_PI*((double)ENC_RADIUS)); // environ : 17.6839

const double ENC_CENTER_DIST_MM = 262.0; // Distance entre chaque roue codeuse
const double ENC_CENTER_DIST_TICKS = ENC_CENTER_DIST_MM*ENC_MM_TO_TICKS; // Distance entre chaque roue codeuse du robot en ticks

// Duree d'un cycle (en millisecondes)
#define DUREE_CYCLE 2

/* Parametres de la carte asservissement
 * pin 0 -> USB 2 TTL
 * pin 1 -> USB 2 TTL
 * pin 3 -> enable moteur 1 (pwm)
 * pin 12 -> direction moteur 1
 * pin 11 -> enable moteur 2 (pwm)
 * pin 13 -> direction moteur 2
 * pin 18 -> A encodeur 1 (gauche)
 * pin 19 -> B encodeur 1 (gauche)
 * pin 21 -> A encodeur 2 (droite)
 * pin 20 -> B encodeur 2 (droite)
 * */
#define PIN_EN_LEFT 3
#define PIN_DIR_LEFT 12
#define PIN_EN_RIGHT 11
#define PIN_DIR_RIGHT 13
#define PIN_LEFT_A 18
#define PIN_LEFT_B 19
#define PIN_RIGHT_A 21
#define PIN_RIGHT_B 20
#define INTERRUPT_LEFT_A 5
#define INTERRUPT_LEFT_B 4
#define INTERRUPT_RIGHT_A 2
#define INTERRUPT_RIGHT_B 3

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
