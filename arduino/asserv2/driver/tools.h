#ifndef _TOOLS_H
#define _TOOLS_H

#define abs(x) ((x)>0 ? (x) : -(x))



/* Fonction de calcul du modulo PI ]-PI,PI]
 * moduloPi( 3*PI/2 )= - PI/2
 * moduloPi( PI ) = PI
 * moduloPi( 0 ) = 0
 *
 * x-y*((int)(x/y)) => c'est le modulo pour des doubles (x%y)
 * trunc et int c'est pareil au final, j'ai choisi trunc juste pour que ce soit plus complique !
 * */
double moduloPI(double Nb);

#endif

