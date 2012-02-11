#include "pid.h"

#include "WProgram.h"

PID::PID(double kp, double ki, double kd)
{
	PID::setCoeffs(kp,ki,kd);
	PID::reset();
}

PID& PID::reset()
{
	PID::resetErrors();
	_lastError = 0.0;

	return (*this);
}

PID& PID::setCoeffs(double kp, double ki, double kd)
{
	_kp = kp;
	_ki = ki;
	_kd = kd;
	
	return (*this);
}

void PID::resetErrors()
{
	for (int i=0; i<NB_ERRORS; ++i)
		_errors[i] = 0.0;
	
	_errSum = 0.0;
	_iErr = 0;
}

PID& PID::setOutputLimits(int a, int b)
{
	_output_lim_a = a;
	_output_lim_b = b;
	
	return (*this);
}


int PID::compute(int error)
{
	int P,I,D;
	int errDiff;
	int pid;
	
    PID::addErr(error);
	
    errDiff = error - _lastError;      //Calcule la variation de l'erreur
    _lastError = error;
	
	P = error * _kp;
	I = _errSum * _ki;
	D = errDiff * _kd;
	
	pid = P + I + D;
	
	if (pid < _output_lim_a)
	{
		pid = _output_lim_a;
	}
	else if (pid > _output_lim_b)
	{
		pid = _output_lim_b;
	}
	
	
	return pid;
}

void PID::addErr(double error)
{
	_errSum += error;
	_errSum -= _errors[_iErr];
	_errors[_iErr] = error;
	
	_iErr++;
	if (_iErr >= NB_ERRORS)
		_iErr = 0;
}


