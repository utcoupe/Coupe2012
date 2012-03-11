/**
 * \file 	global.h
 * \author 	Xavier RODRIGUEZ
 * \date	29/02/2012
 * 
 * */
#ifndef GLOBAL_H
#define GLOBAL_H
 
// Paramétres
#define NB_MAX_ROBOT 4

// Tag des paramétres à passer 
#define TAG_PORTCOM	"-com"
#define TAG_COLOR	"-col"

// --- Définitions ---
	struct coord {
		int x;
		int y;
	};
	
	#define LX 3000			// Largeur et longueur de la table en mm
	#define LY 2000
	#define TETA_DIAG 0.59 	// en radian, sinon 33.69 degree
	#define RAD90 1.57
	#define TOLERANCE 80  	// Tolérence de distance entre deux points du même robot
							// Représente le diametre de la balise posée sur le robot

	#define ABS(a)	   (((a) < 0) ? -(a) : (a))


#endif  //GLOBAL_H
