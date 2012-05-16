#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define SEP			'+' // tout est séparé par ça


//
#define BLEU  	1
#define ROUGE 	2



// all
#define Q_IDENT 			0	// [] identification
#define Q_PING 				1   // [] pong
#define Q_DEBUG				99
#define Q_STOP 				13	// [] stop et vide la fifo

////////////////////////////////////////////////////////////////////////
////						GROSROBOT
////////////////////////////////////////////////////////////////////////

/**
 * identification
 */
#define QO_ID			0

/**
 * pong
 */
#define QO_PING				1

/**
 * Allumer/éteindre les balais
 * @param pwm		[0:255]
 */
#define QO_BALAIS			2

/**
 * Allumer/éteindre le tapis
 * @param on_off		[1:0]
 */
#define QO_TAPIS			3

/**
 * Ouverture peigne
 * @param a angle en degré
 * @param v [0:1023]
 */
#define QO_BOUGER_PEIGNE 	4
/**
 * Ouverture Trappe
 * @param a angle en degré
 * @param v [0:1023]
 */
#define QO_BOUGER_TRAPPE 	5


#define ID_MSG_HOKUYO		104


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
 * @param pwm_left [-255,255]
 * @param pwm_right [-255,255]
 * @param period duree d'application de la pwm
 */
#define QA_PWM				8

/**
 * Stop
 */
#define QA_STOP				9

/**
 * stop et vide la fifo
 */
#define QA_CANCEL 			10


/**
 * Reprendre
 */
#define QA_RESUME			11

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
 * Debug on
 * @param b 0/1
 */
#define QA_DEBUG			22

/**
 * Reset
 */
#define QA_RESET			23

/**
 * @param speed
 */
#define QA_DSPEED			24

/**
 * @param speed
 */
#define QA_ASPEED			25

/**
 * @param p
 * @param i
 * @param d
 */
#define QA_SETPID			26

/**
 *
 */
#define QA_DUMP				27


////////////////////////////////////////////////////////////////////////
////						VISIO
////////////////////////////////////////////////////////////////////////

/**
 * identifiant du client
 * @return 'visio'
 */
#define QV_ID					40


/**
 * ping pong
 * @return 'pong'
 */
#define QV_PING					41


/**
 * passer en mode calibration de la perspective (= détection de l'échéquier)
 * Enreigistrer la matrix dans un fichier warpMatrix.yml
 */
#define QV_CALIB		43


/**
 * arreter le mode calibration de la perspective (= détection de l'échéquier)
 * recharger la matrice perspective depuis le fichier warpMatrix.yml
 */
#define QV_RECHARGE	44


/**
 * récupérer la position des cds et lingots
 * @return ((x1,y1),(x2,y2),(x3,y3),..){SEP}((x1,y1),(x2,y2),(x3,y3),..) cds{SEP}lingots
 */
#define QV_GET				47


////////////////////////////////////////////////////////////////////////
////						HOKUYO
////////////////////////////////////////////////////////////////////////

/**
 * Fonction retournant la postion des robots en coordonnées cartésiennes
 * */
#define QH_GETDATA 				1

/**
 * Dit à l'hokuyo que notre robot est sur la couleur rouge
 * */
#define QH_SET_REDCOLOR 		2

/**
 * Dit à l'hokuyo que notre robot est sur la couleur violet
 * */
#define QH_SET_PURPLECOLOR 		3

/**
 * Ping
 */
#define QH_PING					4

/**
 * Ident
 */
#define QH_IDENT				5

/**
 * Signal pour stoper l'application hokuyo
 * */
#define QH_KILL 9




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
#define E_MERDE_AX18						-59
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






/////////////////////////////////////////////////////////////////////////
//							TESTS
/////////////////////////////////////////////////////////////////////////

/**
 * identifiant du client
 * @return 'visio'
 */
#define QTEST_ID					150


/**
 * ping pong
 * @return 'pong'
 */
#define QTEST_PING					151

/**
 * @param consign
 */
#define QTEST_SPEED					152


/**
 * @param consign
 */
#define QTEST_POSITION				149

/**
 * @param p
 * @param i
 * @param d
 */
#define QTEST_SETVPID				153

/**
 * @param p
 * @param i
 * @param d
 */
#define QTEST_SETPPID				154

/**
 * @param on_off
 */
#define QTEST_DEBUG					155


/**
 */
#define QTEST_STOP					156

/**
 */
#define QTEST_RESUME				157

#endif

