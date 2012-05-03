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

int32_t g_times[g_n] = {0};

/*
#define N	100
int tab[N] = {0};
*/

void dump() {
	/*
	for (int i=0; i<N; ++i) {
		Serial.print("plot ");
		Serial.print(2*i); Serial.print(" ");
		Serial.print(tab[i]); Serial.print(" "); // bleu
		Serial.print(0); Serial.print(" "); // verte
		Serial.println(0); // rouge
	}
	*/
}

void debug(double consign, double current, double pwm) {
	/*
	static int32_t last;
	static double sum_pwm=0, sum_consign=0, sum_current=0;
	static int n=0;
	tab[n] = (int)current;
	++n;
	if (n>=N) n=0;
	*/
}


void computeAlphaDeltaDiff(const int32_t goal_x, const int32_t goal_y, const double goal_a,
	const int32_t current_x, const int32_t current_y, const double current_a,
	double & alpha_diff_alpha, double & alpha_diff_delta, double & delta_diff) {

	const int32_t dx = goal_x-current_x;
	const int32_t dy = goal_y-current_y;
	
	alpha_diff_alpha = moduloPI(alpha_diff(goal_a, current_a));
	alpha_diff_delta = moduloPI(alpha_diff(atan2(dy,dx), current_a));
	delta_diff = sqrt(dx*dx+dy*dy);

	if (abs(alpha_diff_delta) > M_PI_2) {
		delta_diff = -delta_diff;
	}
}

void applyPwm(const int output_alpha, const int output_delta, int& pwm_left, int& pwm_right) {
	pwm_left = output_delta-output_alpha;
	pwm_right = output_delta+output_alpha;
	pwm_left = min(255, max(-255, pwm_left));
	pwm_right = min(255, max(-255, pwm_right));
}

void positionControl(const int32_t goal_x, const int32_t goal_y, const double goal_a,
	const int32_t current_x, const int32_t current_y, const double current_a, const int32_t current_speed, const double current_speed_a,
	int& pwm_left, int& pwm_right, bool & reached) {

	int32_t t;
	
	double delta_diff, alpha_diff_delta, alpha_diff_alpha;
	int output4Delta = 0;
	int output4Alpha = 0;
	

	computeAlphaDeltaDiff(goal_x, goal_y, goal_a, current_x, current_y, current_a,
		alpha_diff_alpha, alpha_diff_delta, delta_diff);

	if(abs(alpha_diff_delta) > M_PI_2) {
		alpha_diff_delta = moduloPI(M_PI + alpha_diff_delta);
	}
	
	

	
	if (abs(delta_diff) > 10.0*ENC_MM_TO_TICKS) {
		t = micros();
		if (alpha_diff_delta < 5.0*DEG_TO_RAD) {
			output4Delta = (int) g_delta_regulator.compute(0.0, (double)-delta_diff, current_speed);
		}
		g_times[4]=micros() - t;

		t = micros();
		output4Alpha = (int) g_alpha_regulator.compute(0.0, (double)-alpha_diff_delta, current_speed_a);
		g_times[5]=micros() - t;
	}
	else {
		reached = true;
	}

	applyPwm(output4Alpha, output4Delta, pwm_left, pwm_right);
}


void angleControl(const int32_t goal_x, const int32_t goal_y, const double goal_a,
	const int32_t current_x, const int32_t current_y, const double current_a, const int32_t current_speed, const double current_speed_a,
	int& pwm_left, int& pwm_right, bool & reached) {

	int32_t t;
	
	double delta_diff, alpha_diff_delta, alpha_diff_alpha, alpha_diff;
	int output4Delta = 0;
	int output4Alpha = 0;
	

	computeAlphaDeltaDiff(goal_x, goal_y, goal_a, current_x, current_y, current_a,
		alpha_diff_alpha, alpha_diff_delta, delta_diff);

	
	
	if (abs(alpha_diff_alpha) > 0.5*DEG_TO_RAD) {
		t = micros();
		output4Alpha = (int) g_alpha_regulator.compute(0.0, (double)-alpha_diff_alpha, current_speed_a);
		g_times[5]=micros() - t;
		
		/*if (abs(delta_diff) > 5.0*ENC_MM_TO_TICKS) {
			t = micros();
			output4Delta = (int) g_delta_regulator.compute(0.0, (double)-delta_diff, current_speed);
			g_times[4]=micros() - t;
		}*/
	}
	else {
		reached = true;
	}

	applyPwm(output4Alpha, output4Delta, pwm_left, pwm_right);
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


