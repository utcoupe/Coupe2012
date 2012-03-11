#ifndef PROTOCOLE_H
#define PROTOCOLE_H
  
// cf. protocole
#define C_SEP_SEND	'.' // tout est séparé par ça.

/**
 * Fonction retournant la postion des robots en coordonnées cartésiennes  
 * */ 
#define QH_GETDATA 1 

/**
 * Dit à l'hokuyo que notre robot est sur la couleur rouge
 * */ 
#define QH_SET_REDCOLOR 2 

/**
 * Dit à l'hokuyo que notre robot est sur la couleur violet
 * */ 
#define QH_SET_PURPLECOLOR 3

/**
 * Signal pour stoper l'application hokuyo
 * */ 
#define QH_KILL 9
 
 
#endif  // PROTOCOLE_H
