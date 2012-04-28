#include "regulator.h"

#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#	include "wiring.h"
#endif

#include "parameters.h"
#include "globals.h"


Regulator::Regulator() {
}

Regulator& Regulator::setPID(double p, double i, double d) {
	_pid.setP(p).setI(i).setD(d);
	return *this;
}

Regulator& Regulator::setOutLimit(double l) {
	_out_limit = l;
	return *this;
}

Regulator& Regulator::setGel(bool b) {
	_pid.setGel(b);
	return *this;
}

double Regulator::compute(double consign, double current) {
	double out = _pid.compute(consign, current);
	
	if (_out_limit > 0) {
		if (out > _out_limit)
			out = _out_limit;
		else if (out < -_out_limit)
			out = -_out_limit;
	}
	
	return out;
}

Regulator& Regulator::reset() {
	_pid.reset();
}

double Regulator::getOutLimit() const {
	return _out_limit;
}


PosAndSpeedRegulator::PosAndSpeedRegulator() : _last_speed_consign(0), _accel_limit(0), _mod(PosAndSpeedRegulator::BOTH), _on(true) {}

PosAndSpeedRegulator& PosAndSpeedRegulator::setMod(PosAndSpeedRegulator::T_MOD mod) {
	_mod = mod;
	return *this;
}

PosAndSpeedRegulator& PosAndSpeedRegulator::setPosPID(double p, double i, double d) {
	_pos_regu.setPID(p,i,d);
	return *this;
}

PosAndSpeedRegulator& PosAndSpeedRegulator::setSpeedPID(double p, double i, double d) {
	_speed_regu.setPID(p,i,d);
	return *this;
}

PosAndSpeedRegulator& PosAndSpeedRegulator::setSpeedLimit(double l) {
	_pos_regu.setOutLimit(l);
	return *this;
}

PosAndSpeedRegulator& PosAndSpeedRegulator::setAccelLimit(double l) {
	//_speed_regu.setOutLimit(l);
	_accel_limit = l;
	return *this;
}

PosAndSpeedRegulator& PosAndSpeedRegulator::setGel(bool b) {
	_pos_regu.setGel(b);
	_speed_regu.setGel(b);
	return *this;
}

PosAndSpeedRegulator& PosAndSpeedRegulator::setOutLimit(double l) {
	_out_limit = l;
	return *this;
}


double PosAndSpeedRegulator::compute(double consign, double current_pos, double current_speed) {
	if (!_on) return 0;
	double r = 0.0;
	switch (_mod)
	{
		case PosAndSpeedRegulator::BOTH:
			r = _bothCompute(consign, current_pos, current_speed);
		break;

		case PosAndSpeedRegulator::SPEED:
			r =  _speedCompute(consign, current_speed);
		break;

		case PosAndSpeedRegulator::POSITION:
			r = _positionCompute(consign, current_pos);
		break;

		default:
			r = 0.0;
		break;
	}

	if (r < -_out_limit) {
		setGel(true);
		r = -_out_limit;
	}
	else if (r > _out_limit) {
		setGel(true);
		r = _out_limit;
	}
	else {
		setGel(false);
	}

	return r;
}

double PosAndSpeedRegulator::_bothCompute(double consign, double current_pos, double current_speed) {
	double speed_consign = _pos_regu.compute(consign, current_pos);
	
	if ((speed_consign - _last_speed_consign) > _accel_limit) {
		speed_consign = _last_speed_consign + _accel_limit;
	}
	else if ((speed_consign - _last_speed_consign) < -_accel_limit) {
		speed_consign = _last_speed_consign - _accel_limit;
	}

	_last_speed_consign = speed_consign;
	return _speed_regu.compute(speed_consign, current_speed);
}

double PosAndSpeedRegulator::_speedCompute(double consign, double current_speed) {
	return _speed_regu.compute(consign, current_speed);
}

double PosAndSpeedRegulator::_positionCompute(double consign, double current_pos) {
	return _pos_regu.compute(consign, current_pos);
}


PosAndSpeedRegulator& PosAndSpeedRegulator::stop() { _on = false; }
PosAndSpeedRegulator& PosAndSpeedRegulator::resume() { _on = true; }
PosAndSpeedRegulator& PosAndSpeedRegulator::reset() {
	_pos_regu.reset();
	_speed_regu.reset();
}


double PosAndSpeedRegulator::getSpeedConsign() const {
	return _last_speed_consign;
}

