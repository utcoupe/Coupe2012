#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define SEP			'+'

/**
 * qui suis-je ?
 */
#define Q_ID			0

/**
 * ping -> pong
 */
#define Q_PING			1

/**
 * Allumer une LED
 * @param i id de la led
 */
#define Q_ALLUME		42


/**
 * Eteindre une LED
 * @param i id de la led
 */
#define Q_ETEINDRE		43

/** PWM
 * @param haut
 * @param gauche
 * @param droite
 * @param bas
 */
#define Q_PWM   44










// erreurs
#define	E_INVALID_CMD						-1
#define	E_INVALID_PARAMETERS_NUMBERS		-2
#define	E_INVALID_PARAMETERS_TYPE			-3
#define E_INVALID_PARAMETERS_VALUE			-4



#endif

