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
#define URG_AUTO_CAPTURE 1
#define REGLAGE_AUTO 1
 
// Libs
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <list>
#include <assert.h>
#include <stdlib.h>

#include "delay.h" 
#include "UrgCtrl.h"

using namespace qrk;
using namespace std; 

// Définitions
struct coord{
	int x;
	int y;
};

#define LX 3000	// Largeur et longueur de la table en mm
#define LY 2000
#define TETA_DIAG 0.59 // en radian, sinon 33.69 degree
#define RAD90 1.57
#define TOLERANCE 50 // Tolérence de distance entre deux points du même robot

#define ABS(a)	   (((a) < 0) ? -(a) : (a))


// Variables globals de controles
bool stop=false;
pthread_mutex_t mutex;

std::list<coord> robot;

UrgCtrl urg;

long indexMax;
long indexMin;
long *distanceMax;


 
#endif  //GLOBAL_H
