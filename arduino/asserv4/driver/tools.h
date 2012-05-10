#ifndef _TOOLS_H
#define _TOOLS_H


#ifndef abs
#	define abs(x) ((x)>0 ? (x) : -(x))
#endif


#include "parameters.h"

/* Fonction de calcul du modulo PI ]-PI,PI]
 * moduloPi( 3*PI/2 )= - PI/2
 * moduloPi( PI ) = PI
 * moduloPi( 0 ) = 0
 *
 * x-y*((int)(x/y)) => c'est le modulo pour des doubles (x%y)
 * trunc et int c'est pareil au final, j'ai choisi trunc juste pour que ce soit plus complique !
 * */
double moduloPI(double Nb);

double alpha_diff(double a, double b);

/////
//			CONVERSIONS
/////
double mm_s_to_ticks_cycle(int u);
double deg_s_to_rad_cycle(int u);

TICKS mm_to_ticks(MM);
MM ticks_to_mm(TICKS);

RAD deg_to_rad(DEG);
DEG rad_to_deg(RAD);


#endif

