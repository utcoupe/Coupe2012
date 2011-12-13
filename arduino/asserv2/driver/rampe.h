#ifndef _RAMPE_H
#define _RAMPE_H


typedef enum {
	PHASE_ACCEL,
	PHASE_CONST,
	PHASE_DECEL,
	PHASE_END
} PHASE;

/*
 * Profile de vitesse en trapèze
 *
 * U : unité arbitraire
 * µs : micro seconde
 * 
 *    v1 ----- v2
 *   /           \
 * v0             v3
 */
class Rampe
{
	public:
		Rampe();
                /**
                  Init the trapezoidal speed profile
                  @param current
                  @param goal
                  @param speed
                  @param accel
                  @param decel
                  @param speed0 initial speed
                  @param speedf final speed
                */
                void compute(double actue, double goal, double goal_speed, double accel, double decel, double speed0=0, double speedf=0);
                /**
                  Perform one step
                  @param dt ellapsed time since last step
                */
                void compute_next_goal(double dt);
                /**
                  @return current position
                */
                double get_pos();
                /**
                  @return current speed
                */
                double get_speed();
                /**
                  @return abs current speed
                */
                double get_aspeed();
                /**
                  @return current phase
                */
		PHASE get_phase();
                /**
                  Change the final speed keeping the others parameters
                */
                void update_speedf(double speedf);

        private:
		/* U position à atteindre */
                double _goal;
                /* U/ms vitesse à atteindre */
                double _goal_speed;
                /* U/ms vitesse finale */
                double _speedf;
                /* U/ms vitesse maximale qui sera atteinte */
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
                /* U/ms vitesse du début de chaque étape */
                double _v0, _v1, _v2, _v3;
		/* U position à chaque changement détape */
		double _pos0, _pos1, _pos2, _pos3;
		/* phase actuelle */
		PHASE _phase;

                bool _no_decel, _no_accel;
};


#endif

