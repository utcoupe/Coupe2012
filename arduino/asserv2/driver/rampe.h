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
                void compute(double actue, double goal, double speed, double accel, double decel, double speed0=0);
                void compute_next_goal(double dt);
		double get_goal();
                double get_speed();
		PHASE get_phase();
                void cancel_decel();

	private:
		int _sens;
		/* U position à atteindre */
                double _goal;
                /* U/ms vitesse à atteindre */
                double _max_speed;
		/* U/ms² accélération voulue */
		double _acc;
		/* U/ms² décélération voulue */
		double _dec;
		/* position actuelle dans la rampe */
                double _current_pos;
		/* vitesse actuelle dans la rampe */
                double _current_speed;
		/* accélération actuelle dans la rampe */
		double _acc_actue;
		/* ms temps actuel */
		double _t;
                /* ms moment du début de chaque étape */
                double _t1, _t2, _t3;
		/* U position à chaque changement détape */
		double _pos0, _pos1, _pos2, _pos3;
		/* phase actuelle */
		PHASE _phase;

                bool _no_decel, _no_accel;
};


#endif

