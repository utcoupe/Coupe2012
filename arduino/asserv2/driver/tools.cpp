#include "tools.h"
#include <math.h>
#include "parameters.h"

double moduloPI(double Nb){
	double result;
	if (Nb > M_PI)
		result = Nb - (2*M_PI)*trunc((Nb + M_PI) / (2*M_PI));
	else if (Nb <= -M_PI)
		result = Nb - (2*M_PI)*trunc((Nb - M_PI) / (2*M_PI));
	else result = Nb;
	return(result);
}


double convert_speed(int speed)
{
	return ((double)speed) * ENC_MM_TO_TICKS / 1000.0;
}


