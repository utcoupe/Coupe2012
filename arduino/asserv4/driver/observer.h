#ifndef OBSERVER_H_
#define OBSERVER_H_


#include "parameters.h"


class RobotObserver
{
	public:
		RobotObserver();

		void compute(int32_t left_value, int32_t right_value);
		void reset();
		void setPos(TICKS x, TICKS y, RAD a);
		
		TICKS x() const;
		TICKS y() const;
		MM mm_x() const;
		MM mm_y() const;
		RAD a() const;
		DEG deg_a() const;
		double getSpeed() const;
		double mm_s_getSpeed() const;
		double getSpeedA() const;
		double deg_s_getSpeedA() const;

	private:
		TICKS _prev_left_value, _prev_right_value;
		TICKS_100 _x;
		TICKS_100 _y;
		RAD _a;
		
		// {ticks/cycle}
		double _speed;
		// {radians/cycle}
		double _speed_a;
};

#endif

