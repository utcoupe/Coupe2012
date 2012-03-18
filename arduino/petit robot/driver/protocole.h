#ifndef PROTOCOLE_H
#define PROTOCOLE_H


// cf. protocole
#define SEP                     '+'

/**
 * Passe en mode buldozer
 */
#define Q_BULDOZER_START        10
// paramètre @duree pour arrêter automatiquement
//  le mode buldozer ?

/**
 * Arrête le mode buldozer
 */
#define Q_BULDOZER_STOP         20

/**
 * Sors les balais
 */
#define Q_SORTIR_BALAIS         30
// pareil que buldozer_start

/**
 * Rentre les balais
 */
#define Q_RENTRE_BALAIS         40



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

