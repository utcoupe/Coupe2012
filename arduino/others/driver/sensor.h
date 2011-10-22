#ifndef SENSOR_H_
#define SENSOR_H_

#include "WProgram.h"
#include "message.h"
#include "ping.h"

// interrupts : 2 (interrupt 0), 3 (interrupt 1), 18 (interrupt 5), 19 (interrupt 4), 20 (interrupt 3), and 21 (interrupt 2)

//define microswitch
#define PIN_MS_AV 	48
#define PIN_MS_AR 	50
#define NB_MS 		2

//define jack et couleur
#define PIN_JACK 		35
#define INTERRUPT_JACK	1
#define LED_BLEU 		29
#define LED_ROUGE 		33
#define BLEU 			0
#define ROUGE 			1


// pin couleur
#define PIN_COLOR		31
#define INTERRUPT_COLOR	0

//define sharp
#define MAX_SEUIL 	450 //valeur maximum pour un seuil de sharp ==> distance minimum de detection
#define NB_SHARP 	12
//sharp type 1
#define DIST_MAX1	10 //valeur sharp petit = loin
#define DIST_MIN1	400
//sharp type 2
#define DIST_MAX2	100 //valeur sharp petit = loin
#define DIST_MIN2	400
//numero de pin des sharp 0 <=> A0
//num 1 en bas num 5 en haut
//Face avant
#define SHARP_AV1	0
#define SHARP_AV2D	1 //droite
#define SHARP_AV2G	2 //gauche
#define SHARP_AV3	3
#define SHARP_AV4	4
#define SHARP_AV5	5
//face arriere
#define SHARP_AR1	6
#define SHARP_AR2D	7
#define SHARP_AR2G	8
#define SHARP_AR3	9
#define SHARP_AR4	10
#define SHARP_AR5	11
//PING
#define PIN_PING_AV 51
#define PIN_PING_AR 53
#define DISTANCE_DETECT 40//cm


struct TriggerSharp{
	int messageId;
	int pin;
	int value;
};



extern int pingAvMessageID;
extern int pingArMessageID;

void initSensor();
int setLED( char color);
int getPion( char face);
int getSharp( char pin);
int setTriggerSharp( int id,  char pin, int ref);
void removeTriggerSharp( char index);
int getMicroSwitch( char pin);
int setTriggerMS( int id,  char pin,bool ref);
void removeTriggerMS( int index);
void sensorTrigger();
int getColor();
void valueChangeOnJack();
void valueChangeOnSwitchColor();

#endif
