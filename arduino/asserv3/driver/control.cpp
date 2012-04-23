#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "wiring.h"
#	include "WProgram.h"
#endif

#include "control.h"
#include "globals.h"
#include <math.h>
#include "tools.h"


#define N	100
int tab[N] = {0};

void dump() {
	for (int i=0; i<N; ++i) {
		Serial.print("plot ");
		Serial.print(2*i); Serial.print(" ");
		Serial.print(tab[i]); Serial.print(" "); // bleu
		Serial.print(0); Serial.print(" "); // verte
		Serial.println(0); // rouge
	}
}

void debug(double consign, double current, double pwm) {
	static int32_t last;
	static double sum_pwm=0, sum_consign=0, sum_current=0;
	static int n=0;
	tab[n] = (int)current;
	++n;
	if (n>=N) n=0;
	//int32_t t = millis();
		
	/*++n;
	sum_consign += consign;
	sum_current += current;
	sum_pwm += pwm;*/
	/*if (g_debug_on and (t - last) > 5) {
		Serial.println("--- coucou ---");
		Serial.print(G_value_left_enc); Serial.print(" "); Serial.println(G_value_right_enc);
		Serial.print("plot ");
		Serial.print(millis()); Serial.print(" ");
		Serial.print(consign); Serial.print(" "); // bleu
		Serial.print(current); Serial.print(" "); // verte
		Serial.println(0); // rouge
		
		last = t;
		n = sum_consign = sum_current = sum_pwm = 0;
	}*/
}





void positionControl(bool goal_position, int32_t goal_x, int32_t goal_y, double goal_a,
	int32_t current_x, int32_t current_y, double current_a, int32_t current_speed, double current_speed_a,
	int& pwm_left, int& pwm_right) {
	
	pwm_left = 0;
	pwm_right = 0;
	int output4Delta = 0;
	int output4Alpha = 0;
	int32_t dx = goal_x-current_x;
	int32_t dy = goal_y-current_y;
	
	double alpha_diff_alpha = alpha_diff(goal_a, current_a);
	double alpha_diff_delta = alpha_diff(atan2(dy, dx), current_a);
	
	double alpha_diff = 0.0;

	if (goal_position) {
		alpha_diff = alpha_diff_delta;
	}
	else {
		alpha_diff = alpha_diff_alpha;
	}

	int sens_delta=1;
	// Si le goal est derrière
	if(abs(alpha_diff_delta) > M_PI/2) {
		sens_delta = -sens_delta;
		if (goal_position) {
			alpha_diff = moduloPI(M_PI + alpha_diff);
		}
	}

	double delta_diff = sqrt(dx*dx+dy*dy);
	
	double current_delta = sens_delta * delta_diff;
	double current_alpha = alpha_diff;

	if (((!goal_position and fabs(alpha_diff) < DEG_TO_RAD) or
		(goal_position and fabs(delta_diff) < 10*ENC_MM_TO_TICKS)))
	{
		// goal atteind
	}
	else {
		output4Delta = (int) g_delta_regulator.compute(0.0, (double)-current_delta, current_speed);
		output4Alpha = (int) g_alpha_regulator.compute(0.0, (double)-current_alpha, current_speed_a);

		pwm_left = output4Delta-output4Alpha;
		pwm_right = output4Delta+output4Alpha;
		pwm_left = min(255, max(-255, pwm_left));
		pwm_right = min(255, max(-255, pwm_right));
	}

	/*if ((millis()%500) == 0) {
		Serial.println("coucou");
		Serial.print("pwm_left"); Serial.println(pwm_left);
		Serial.print("pwm_right"); Serial.println(pwm_right);
		Serial.print("out_delta"); Serial.println(output4Delta);
		Serial.print("out_alpha"); Serial.println(output4Alpha);
		Serial.print("delta"); Serial.println(current_delta);
		Serial.print("alpha"); Serial.println(current_alpha);
	}//*/
	
	debug(0.0, current_alpha*1000.0, pwm_right);
}




void alphaSpeedControl(double goal_speed, double current_speed, PosAndSpeedRegulator& regul,
	int& pwm_left, int& pwm_right) {

	
	pwm_right = (int) regul.compute(goal_speed, 0, current_speed);
	pwm_left = -pwm_right;

	debug(goal_speed*1000000.0, current_speed*1000000.0, pwm_right);
}

void deltaSpeedControl(double goal_speed, double current_speed, PosAndSpeedRegulator& regul,
	int& pwm_left, int& pwm_right) {

	pwm_right = (int) regul.compute(goal_speed, 0, current_speed);
	pwm_left = pwm_right;
	
	debug(goal_speed*100.0, current_speed*100.0, pwm_right);
}


