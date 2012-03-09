#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define SEP			'+' // tout est séparé par ça
#define C_SEP_SEND	SEP 


// all
#define Q_IDENT 			0	// [] identification
#define Q_PING 				1   // [] pong
#define Q_DEBUG				99
#define Q_STOP 				13	// [] stop et vide la fifo

////////////////////////////////////////////////////////////////////////
////						OTHERS
////////////////////////////////////////////////////////////////////////

/**
 * identification
 */
#define QO_IDENT			0

/**
 * pong
 */
#define QO_PING				1

/**
 * Fait des resets
 */
#define QO_RESET		 	45


////////////////////////////////////////////////////////////////////////
////						ASSERV
////////////////////////////////////////////////////////////////////////
/**
 * identification
 */
#define QA_ID 				0

/**
 * ping-pong
 */
#define QA_PING 			1

/**
 * stop et vide la fifo
 */
#define QA_CANCEL 			13

/**
 * @param x
 * @param y
 * @param vitesse
 */
#define QA_GOTO		 		2

/**
 * @param x
 * @param y
 * @param vitesse
 */
#define QA_GOTOR	 		3

/**
 * @param angle
 * @param vitesse
 */
#define QA_TURN		 		4

/**
 * Tourner en relatif
 * @param angle
 * @param vitesse
 */
#define QA_TURNR	 		5

/**
 * Position actuelle du robot
 */
#define QA_POS		 		6

/**
 * Définir la position actuelle
 * @param x mm
 * @param y mm
 * @param a degrès
 */
#define QA_SET_POS			7

/**
 * Mettre une pwm à droite et à gauche
 * @param pwm [-255,255]
 */
#define QA_PWM				8

/**
 * Debug : sens du robot.
 */
#define QA_GETSENS			16

/**
 * Debug : valeur des encodeurs.
 */
#define QA_GETENC			17

/**
 * Debug : set les valeurs du PID pour l'asservissement des goto (delta)
 * @param P (1/1000)
 * @param I (1/1000)
 * @param D (1/1000)
 */
#define QA_SET_PID_POSITION_D	18

/**
 * Debug : set les valeurs du PID pour l'asservissement des goto (alpha)
 * @param P (1/1000)
 * @param I (1/1000)
 * @param D (1/1000)
 */
#define QA_SET_PID_POSITION_A	19

/**
 * Debug : set les valeurs du PID pour l'asservissement des rotations (delta)
 * @param P (1/1000)
 * @param I (1/1000)
 * @param D (1/1000)
 */
#define QA_SET_PID_ANGLE_D	20

/**
 * Debug : set les valeurs du PID pour l'asservissement des rotations (alpha)
 * @param P (1/1000)
 * @param I (1/1000)
 * @param D (1/1000)
 */
#define QA_SET_PID_ANGLE_A	21

/**
 * Stop
 */
#define QA_STOP				22


/**
 * Reprendre
 */
#define QA_RESUME			23


// erreurs
#define	E_INVALID_CMD						-1
#define	E_INVALID_PARAMETERS_NUMBERS		-2
#define	E_INVALID_PARAMETERS_TYPE			-3
#define E_INVALID_PARAMETERS_VALUE			-4
#define E_TOURELLE_SETUP_FAIL				-5
#define W_SWITCH_COLOR						-21
#define W_JACK								-22
#define W_PING_AV							-30
#define W_PING_AR							-31
#define W_STOP								-42
#define E_TIMEOUT							-43
#define E_BLOCK								-44
#define E_SURCHAUFFE						-55
#define E_LOW_VOLTAGE						-56
#define E_MERDE_AX12						-57
#define E_CM5_NOT_CONN						-58
#define W_MS								-60
#define Q_KILL								-69


// les ids
#define ID_SERVER		0
#define ID_CAM			1
#define ID_AX12			2
#define ID_ASSERV		3 // carte asserv
#define ID_OTHERS		4 // carte avec les pinces et sharps
#define ID_IA			5 // l'IA

// paramètres
#define AVANT		0
#define ARRIERE		1
#define ARRET		2
// pour les ascenseurs
#define HAUT		1
#define BAS			2
#define MIDLE		3

#define GAUCHE		0
#define DROITE		1

#endif

