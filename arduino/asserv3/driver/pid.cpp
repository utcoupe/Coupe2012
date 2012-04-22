#include "pid.h"

#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#	include "wiring.h"
#endif

PID::PID(double kp, double ki, double kd) :
	_kp(kp), _ki(ki), _kd(kd),
	_err_sum(0), _last(0), _tab_errs({0}), _i_tab_errs(0),
	_min(-(1L<<30)), _max(1L<<30), _gel_active(false) {}

PID& PID::reset() {
	_err_sum = 0;
	_tab_errs = {0};
	return *this;
}

double PID::computeErrSum(double err) {
	if (_gel_active)
		return 0;
	
	// sauvegarde de l'erreur
	_tab_errs[_i_tab_errs] = err;

	// incrément de l'index
	++_i_tab_errs;
	if (_i_tab_errs == NB_ERRORS) {
		_i_tab_errs = 0;
	}

	// modification de la somme
	// nouvelle_somme = ancienne_somme + nouvelle_erreur - plus_ancienne_erreur
	_err_sum += err - _tab_errs[_i_tab_errs];

	return _err_sum;
}

double PID::compute(double consign, double current) {
	double err = consign - current;		// calcul erreur
	double err_sum = computeErrSum(err);						// calcul intégrale
	double delta_current = current - _last;	// dérivée de la sortie du process
	//double delta_err = err - _last;
	double out = (_kp * err) + (_ki * _err_sum / NB_ERRORS) - (_kd * delta_current);	// PID
	_last = current;					// sauvegarde
	//double out = (_kp * err) + (_ki * _err_sum / NB_ERRORS) + (_kd * delta_err);	// PID
	//_last = err;					// sauvegarde

	return out;
}

PID& PID::setP(double p) { _kp = p; return *this; }

PID& PID::setI(double i) { _ki = i; return *this; }

PID& PID::setD(double d) { _kd = d; return *this; }

PID& PID::setGel(bool b) { _gel_active = b; return *this; }

