#ifndef GOAl_H
#define GOAL_H

#include "parameters.h"

class Goal {
	public:
		typedef enum {GOAL_POS, GOAL_ANG, GOAL_DELTA_SPEED, GOAL_ALPHA_SPEED, GOAL_PWM} GOAL_TYPE;
		static Goal* get();
		void set(GOAL_TYPE t, TICKS x, TICKS y, RAD a, double speed, double speed_a, int id, bool reached=false);
		int32_t x() const;
		int32_t y() const;
		double a() const;
		double speed() const;
		double speed_a() const;
		GOAL_TYPE t() const;
		bool reached() const;
		int id() const;
		void reset();
		void setReached(bool b);

	private:
		Goal();
		static Goal * _goal;
		GOAL_TYPE _t;
		TICKS _x;
		TICKS _y;
		double _speed;
		double _speed_a;
		RAD _a;
		bool _reached;
		int _id;
};



#endif

