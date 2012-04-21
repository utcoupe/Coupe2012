#ifndef OBSERVER_H_
#define OBSERVER_H_


#include <inttypes.h>



class RobotObserver
{
	public:
		RobotObserver();

		void compute(int32_t left_value, int32_t right_value);
		void reset();

		int32_t getX() const;
		int32_t getY() const;
		int32_t mm_getX() const;
		int32_t mm_getY() const;
		double getA() const;
		int deg_getA() const;
		int32_t getSpeed() const;
		double mm_s_getSpeed() const;
		double getSpeedA() const;
		double deg_s_getSpeedA() const;

	private:
		int32_t _prev_left_value, _prev_right_value;
		int32_t _x, _y, _speed;
		double _a, _speed_a;
};

#endif

