#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define C_SEP_SEND	'.' // tout est séparé par ça.



// all
#define Q_IDENT 			0
#define Q_PING 				1
#define Q_DEBUG				99
#define Q_STOP 				13

// autre
#define Q_IDENT 			0
#define Q_PING 				1
#define Q_SHARP 			31
//#define Q_TSHARP 			32 //TRIGGERSHARP //obselete
#define Q_MS	 			33
#define Q_TMS	 			34 //TRIGGERMS
//#define Q_PINCE 			35
#define Q_SETPOSITION 		36 //PINCE POSITION
#define Q_LOAD 				37 //PRESENT_LOAD
#define Q_JACK		 		38
#define Q_LED		 		39
#define Q_PION		 		40
#define Q_RESET		 		45
#define Q_RECALAGE	 		46
#define Q_COLOR				50
#define Q_ULTRAPING			51
#define Q_GETULTRAPING		52
#define Q_CODEUR	 		44 

// asserv
#define Q_GOAL_ABS 			2
#define Q_GOAL_REL 			3
#define Q_ANGLE_ABS 		4
#define Q_ANGLE_REL 		5
#define Q_POSITION 			6
#define Q_AUTO_CALIB		7
#define Q_MANUAL_CALIB		8
#define Q_DELAY 			9
#define Q_PWM 				10
#define Q_MODIF_GOAL_ABS 	11
#define Q_MODIF_GOAL_REL 	12
#define Q_PAUSE				14
#define Q_RESUME			15
#define Q_GETSENS			16
#define Q_GETENC			17

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

