#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define SEP                     '+'

/**
Renvoie l'entier passé en paramètre
@param entier
*/
#define Q_PING			10

/**
Va à une certaine position angulaire (en °)
@param moteur
@param position
*/
#define Q_TOURNER		50

/**
Allume le moteur du percepteur
@param moteur
@param pwm [-255, 255]
*/
#define Q_PERCEPTEUR		60

/**
Renvoie la position angulaire du moteur
@param moteur
*/
#define Q_GET_POS		70

/**
   Renvoie la position angulaire du moteur
   @param info
   @param moteur
*/
#define Q_LIRE		80

/**
   Attrape la carte
*/
#define Q_ATTRAPER_CARTE 90

/**
   Passe en mode buldozer
*/
#define Q_GO_BULDOZER 100

/**
   Arrête le mode buldozer
*/
#define Q_STOP_BULDOZER 110

// erreurs
#define E_INVALID_CMD                   -1
#define E_INVALID_PARAMETERS_NUMBERS    -2
#define E_INVALID_PARAMETERS_TYPE       -3
#define E_INVALID_PARAMETERS_VALUE      -4
#define E_TOURELLE_SETUP_FAIL           -5
#define W_SWITCH_COLOR                  -21
#define W_JACK                          -22
#define W_PING_AV                       -30
#define W_PING_AR                       -31
#define W_STOP                          -42
#define E_TIMEOUT                       -43
#define E_BLOCK                         -44
#define E_SURCHAUFFE                    -55
#define E_LOW_VOLTAGE                   -56
#define E_MERDE_AX12                    -57
#define E_CM5_NOT_CONN                  -58
#define W_MS                            -60
#define Q_KILL                          -69



#endif

