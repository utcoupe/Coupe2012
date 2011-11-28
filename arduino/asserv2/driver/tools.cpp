#include "tools.h"
#include <math.h>

double moduloPI(double Nb){
	double result;
	if (Nb > M_PI)
		result = Nb - (2*M_PI)*trunc((Nb + M_PI) / (2*M_PI));
	else if (Nb <= -M_PI)
		result = Nb - (2*M_PI)*trunc((Nb - M_PI) / (2*M_PI));
	else result = Nb;
	return(result);
}
