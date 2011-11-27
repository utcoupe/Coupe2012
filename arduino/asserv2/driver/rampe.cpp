#include "rampe.h"

#include <math.h>

#define abs(x) ((x>0) ? x : (-x))


Rampe::Rampe()
{


}


void Rampe::compute(long int actue, long int goal, long int speed, long int accel, long int decel)
{
	long int time_acc = (long int) (speed / accel);
	long int d_acc = (long int) (accel * time_acc * time_acc / 2.0);
	long int time_dec = (long int) (-speed / decel);
	long int d_dec = (long int) (-decel * time_dec * time_dec / 2.0);
	long int d_const = abs(goal - actue) - (d_acc + d_dec);

	// cas limite quand la vitesse max ne peut être atteinte
	if (d_const < 0)
	{
		time_dec = sqrt(abs(goal - actue) / ((-decel / (2.0*accel) + 0.5) * -decel));
		speed = -decel * time_dec;
		d_acc = accel * time_acc * time_acc / 2.0;
		time_acc = speed / accel;
		d_acc = accel * time_acc * time_acc / 2.0;
		d_const = 0;
	}
	long int time_const = (long int) (d_const / speed);

	if (goal > actue)
		_sens = 1;
	else
		_sens = -1;
	
	// les goals
	_goal = goal;
	_speed = speed;
	_acc = accel;
	_dec = decel;
	// les valeurs actueelles théoriques
	_pos_actue = actue;
	_speed_actue = 0;
	_acc_actue = 0;
	_t = 0;
	// temps de chaque début de phase
	_t01 = time_acc;
	_t12 = _t01 + time_const;
	_t23 = _t12 + time_dec;
	// position de chaque début de phase
	_pos0 = actue;
	_pos1 = _pos0 + _sens * d_acc;
	_pos2 = _pos1 + _sens * d_const;
	_pos3 = goal;
	_phase = PHASE_ACCEL;
}

void Rampe::compute_next_goal(long dt)
{
	_t += dt;
	long t;

	switch (_phase)
	{
		case PHASE_ACCEL:
		{
			t = _t;
			_acc_actue = _acc;
			_speed_actue = _acc * t;
			_pos_actue = _pos0 + _sens * _acc * t*t / 2.0;
			if (_t >= _t01)
			{
				_phase = PHASE_CONST;
			}
			break;
		}
		case PHASE_CONST:
		{
			t = _t - _t01;
			_acc_actue = 0;
			_speed_actue = _speed;
			_pos_actue = _pos1 + _speed * t;
			if (_t >= _t12)
			{
				_phase = PHASE_DECEL;
			}
			break;
		}
		case PHASE_DECEL:
		{
			t = _t - _t12;
			_acc_actue = _dec;
			_speed_actue = _speed + _dec * t;
			_pos_actue = _pos2 + _sens * _speed * t + _sens * _dec * t*t / 2.0;
			if (_t >= _t23)
				_phase = PHASE_END;
			break;
		}
		case PHASE_END:
		{
			_acc_actue = 0;
			_speed_actue = 0;
			_pos_actue = _pos3;
			break;
		}
	}
}

long Rampe::get_goal()
{
	return _pos_actue;
}


PHASE Rampe::get_phase()
{
	return _phase;
}



	
