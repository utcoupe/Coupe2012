/**
 * \file 	global.h
 * \author 	Xavier RODRIGUEZ
 * \date	29/02/2012
 * 
 * */
#ifndef GLOBAL_H
#define GLOBAL_H
 
// Paramétres
#define DEBUG 0
#define NB_MAX_ROBOT 4
 
// Libs
#include <list>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>

#include "delay.h" 
#include "UrgCtrl.h"

using namespace qrk;
using namespace std; 

// --- Définitions ---
	struct coord{
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

	// 
	#define VIOLET 1
	#define ROUGE  2
	
	//
	const long 	min_length=20;
	const long 	max_length=3000;

	// 
	short 		g_color;
	string 		g_comPort;
	
	//
	UrgCtrl 	g_urg;
	int 		g_scanMsec;

	//
	double 		g_radMin;
	double 		g_radMax;
		
	long 		g_indexMax;
	long 		g_indexMin;

	long* 		g_distanceMax;
	
	
	//
	bool g_stop=false;
	pthread_mutex_t  mutex;
	std::list<coord> robot;


#endif  //GLOBAL_H
