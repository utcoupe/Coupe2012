#ifndef PINCECONTROL_H_
#define PINCECONTROL_H_

#include "AFMotor.h"
#include "message.h"
#include "pwm.h"

#define BAUDRATE 	115200

//Pinces
//#define HAUT FORWARD
//#define BAS BACKWARD


//deplacement verticale
#define POSITION_MAX 11000
#define PWM_MOVE 		250 // PWM pour se déplacer rapidement
#define PWM_MAINTIENT	100 // PWM pour le maintient en position
#define MARGE_MARCHE	500 // Tolérance avant de ralentir
#define MARGE_MAINTIENT	50 // Tolerance de maintient en position pour la pince
#define SEUIL_LOAD	0x50
#define PIN_MS_RECAL_AV			30 //microswitch pour recalage
#define PIN_MS_RECAL_AR			37
#define PIN_MS_RECAL_AV_HAUT	21
#define PIN_MS_RECAL_AV_BAS		20
#define PIN_MS_RECAL_AR_HAUT	18
#define PIN_MS_RECAL_AR_BAS		19

extern int msg_position_AV,msg_position_AR;

#include "encoder.h"

void initPinceControl();
//int setPinceState( char index, char etat);
int setPincePosition(int id, char, int pos);
int pinceRecal(int id, char face);

#endif
