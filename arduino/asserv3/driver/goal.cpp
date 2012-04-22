#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "wiring.h"
#	include "WProgram.h"
#endif
#include "goal.h"

Goal* Goal::_goal = 0;

Goal* Goal::get() {
	if (_goal==0) {
		_goal = (Goal*)calloc(1, sizeof(Goal));
	}

	return _goal;
}


Goal::Goal() : _x(0), _y(0), _a(0.0), _t(GOAL_POS), _speed(0.0), _speed_a(0.0) {}

void Goal::set(GOAL_TYPE t, TICKS x, TICKS y, RAD a, double speed, double speed_a) {
	_t = t;
	_x = x;
	_y = y;
	_a = a;
	_speed = speed;
	_speed_a = speed_a;
}

void Goal::reset() {
	_a = _speed_a = _speed = _x = _y = 0;
	_t = GOAL_POS;
}

int32_t Goal::x() const { return _x; }
int32_t Goal::y() const { return _y; }
double Goal::a() const { return _a; }
double Goal::speed() const { return _speed; }
double Goal::speed_a() const { return _speed_a; }
Goal::GOAL_TYPE Goal::t() const { return _t; }
