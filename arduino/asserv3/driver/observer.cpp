#include "observer.h"

#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#	include "wiring.h"
#endif

#include "math.h"

#include "parameters.h"
#include "tools.h"


RobotObserver::RobotObserver() : _x(0), _y(0), _speed(0), _a(0.0), _speed_a(0.0), _prev_left_value(0), _prev_right_value(0) {};

void RobotObserver::compute(int32_t left_value, int32_t right_value) {

	TICKS dl = left_value - _prev_left_value;
	TICKS dr = right_value - _prev_right_value;
	
	// preparation de la prochaine iteration
	_prev_left_value = left_value;
	_prev_right_value = right_value;


	// vitesse de rotation
	_speed_a = (dr-dl)/(double)ENC_CENTER_DIST_TICKS;
	//_speed_a = atan2(dr-dl, ENC_CENTER_DIST_TICKS);

	// vitesse
	_speed = (dr+dl)/2.0;

	
	// mise a jour de l'etat du robot
	_a =  moduloPI(_a + _speed_a);	
	_x += _speed*cos(_a);//(1.0-0.5*0.5*_a);//cos(_a);
	_y += _speed*sin(_a);//(_a-_a*_a*_a/6.0);//sin(_a);

}

TICKS RobotObserver::getX() const {
	return _x;
}

TICKS RobotObserver::getY() const {
	return _y;
}

MM RobotObserver::mm_getX() const {
	return ticks_to_mm(_x);
}

MM RobotObserver::mm_getY() const {
	return ticks_to_mm(_y);
}

RAD RobotObserver::getA() const {
	return _a;
}

DEG RobotObserver::deg_getA() const {
	return rad_to_deg(_a);
}

double RobotObserver::getSpeed() const {
	return _speed;
}

double RobotObserver::mm_s_getSpeed() const {
	return (double)_speed * ENC_TICKS_TO_MM * 1000.0 / DUREE_CYCLE;
}

double RobotObserver::getSpeedA() const {
	return _speed_a;
}

double RobotObserver::deg_s_getSpeedA() const {
	return _speed_a * RAD_TO_DEG * 1000.0 / DUREE_CYCLE;
}

void RobotObserver::reset() {
	_x = _y = _speed = _a = _speed_a = _prev_left_value = _prev_right_value = 0;
}

void RobotObserver::setPos(TICKS x, TICKS y, RAD a) {
	_x = x;
	_y = y;
	_a = a;
}



