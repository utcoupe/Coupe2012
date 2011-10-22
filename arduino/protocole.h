#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define C_SEP_SEND	'.' // tout est séparé par ça.
#define V_SEP		'.'


// all
#define Q_IDENT 			0 // [] identification
#define Q_PING 				1   // [] pong
#define Q_DEBUG				99
#define Q_STOP 				13 // [] stop et vide la fifo

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
 * Valeur d'un microswitch
 * @param face 0:AV 1:AR
 */
#define QO_MS	 			33

/**
 * trigger microswitch
 * @param face 0:AV 1:AR
 */
#define QO_TMS	 			34

/**
 * Fixer la position des pinces
 * @param face 0:AV 1:AR
 * @param hauteur
 */
#define QO_SETPPOSITION 	36
#define Q_JACK		 		38

/**
 * Fait des resets (see PH for more details)
 */
#define QO_RESET		 	45

/**
 * Recaler la pince n
 * @param face 0:AV 1:AR
 */
#define QO_PRECALAGE	 	46

/**
 * Retourne la couleur du robot
 */
#define QO_COLOR				50

/**
 * Tourne la tourelle
 * @param face 0:AV 1:AR
 */
#define QO_ULTRAPING		51

/**
 * Retourne la valeure du ping
 * @param face 0:AV 1:AR
 */
#define QO_GETULTRAPING		52

/**
 * Debug : valeur du codeur de l'ascenseur d'une pince
 * @param face 0:AV 1:AR
 */
#define QO_PCODEUR	 		44

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
 * Recalage automatique
 * @param color 0:blue 1:rouge
 */
#define QA_ACALIB			7
#define QA_MCALIB			8
#define QA_DELAY 			9
#define QA_PWM 				10
#define QA_MODIF_GOAL_ABS 	11
#define QA_MODIF_GOAL_REL 	12

/**
 * Pause.
 */
#define QA_STOP				14

/**
 * Reprendre.
 */
#define QA_RESUME			15

/**
 * Debug : sens du robot.
 */
#define QA_GETSENS			16

/**
 * Debug : valeur des encodeurs.
 */
#define QA_GETENC			17

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

