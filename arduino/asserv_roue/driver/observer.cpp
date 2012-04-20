#include "observer.h"

#include "parameters.h"

PositionObserver::PositionObserver() : _last_position(0), _last_speed(0), _x(0), _v(0), _a(0) {}

void PositionObserver::compute(int32_t current) {
	_x = current;
	_v = _x - _last_position;
	_a = _v - _last_speed;

	_last_position = _x;
	_last_speed = _v;
}

int32_t PositionObserver::getX() const { return _x; }
int32_t PositionObserver::getV() const { return _v; }
int32_t PositionObserver::getA() const { return _a; }

double PositionObserver::mm_getX() const {
	return (double)_x * ENC_TICKS_TO_MM;
}

double PositionObserver::mm_s_getV() const {
	return (double)_v * (double)ENC_TICKS_TO_MM * (double)1000.0 / (double)DUREE_CYCLE;
}



