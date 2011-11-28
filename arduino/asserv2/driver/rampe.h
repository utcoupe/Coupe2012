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
		void compute(double actue, double goal, double speed, double accel, double decel);
		void compute_next_goal(long dt);
		double get_goal();
		PHASE get_phase();

	private:
		int _sens;
		/* U position à atteindre */
		double _goal;
		/* U/ms vitesse à atteindre */
		double _speed;
		/* U/ms² accélération voulue */
		double _acc;
		/* U/ms² décélération voulue */
		double _dec;
		/* position actuelle dans la rampe */
		double _pos_actue;
		/* vitesse actuelle dans la rampe */
		double _speed_actue;
		/* accélération actuelle dans la rampe */
		double _acc_actue;
		/* ms temps actuel */
		double _t;
		/* ms temps de chaque étape */
		double _t01, _t12, _t23;
		/* U position à chaque changement détape */
		double _pos0, _pos1, _pos2, _pos3;
		/* phase actuelle */
		PHASE _phase;
};


#endif

