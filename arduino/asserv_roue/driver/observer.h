#ifndef OBSERVER_H_
#define OBSERVER_H_


#include <inttypes.h>


class PositionObserver
{
	public:
		PositionObserver();
		void compute(int32_t current);

		int32_t getX() const;
		int32_t getV() const;
		int32_t getA() const;

		double mm_getX() const;
		double mm_s_getV() const;

	private:
		int32_t _last_position;
		int32_t _last_speed;

		int32_t _x, _v, _a;
};

#endif

