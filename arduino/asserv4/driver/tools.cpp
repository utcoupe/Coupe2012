#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#	include "wiring.h"
#endif
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


double alpha_diff(double a, double b)
{
	//return atan2(sin(a-b), cos(a-b));
	return moduloPI(a - b);
}


double mm_s_to_ticks_cycle(int u) {
	return u * ENC_MM_TO_TICKS * DUREE_CYCLE / 1000.0;
}

double deg_s_to_rad_cycle(int u) {
	return u * DEG_TO_RAD * DUREE_CYCLE / 1000.0;
}



TICKS mm_to_ticks(MM mm) { return (TICKS) (((double)mm) * (double)ENC_MM_TO_TICKS); }
MM ticks_to_mm(TICKS ticks) { return (MM) (((double)ticks) * (double)ENC_TICKS_TO_MM); }

RAD deg_to_rad(DEG a) { return (RAD) (((double)a) * (double)DEG_TO_RAD); }
DEG rad_to_deg(RAD a) { return (DEG) (((double)a) * (double)RAD_TO_DEG); }


