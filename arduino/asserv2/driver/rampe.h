#ifndef _RAMPE_H
#define _RAMPE_H


typedef enum {
	PHASE_ACCEL,
	PHASE_CONST,
	PHASE_DECEL,
	PHASE_END
} PHASE;

/*
 * U : unité arbitraire
 * µs : micro seconde
 * 
 *    t1 ----- t2
 *   /           \
 * t0             t3
 */
class Rampe
{
	public:
		Rampe();
		void compute(int actue, int goal, int speed, int accel, int decel);
		void compute_next_goal(long dt);
		long get_goal();
		PHASE get_phase();

	private:
		int _sens;
		/* U position à atteindre */
		int _goal;
		/* U/µs vitesse à atteindre */
		int _speed;
		/* U/µs² accélération voulue */
		int _acc;
		/* U/µs² décélération voulue */
		int _dec;
		/* position actuelle dans la rampe */
		int _pos_actue;
		/* vitesse actuelle dans la rampe */
		int _speed_actue;
		/* accélération actuelle dans la rampe */
		int _acc_actue;
		/* µs temps actuel */
		long _t;
		/* µs temps de chaque étape */
		long _t01, _t12, _t23;
		/* U position à chaque changement détape */
		int _pos0, _pos1, _pos2, _pos3;
		/* phase actuelle */
		PHASE _phase;
};


#endif

