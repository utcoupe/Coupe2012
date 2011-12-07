#include "rampe.h"
#include "WProgram.h"
#include <math.h>



Rampe::Rampe()
{


}


void Rampe::compute(double actue, double goal, double speed, double accel, double decel, double current_speed)
{
	Serial.println("rampe");
	Serial.println(actue);
	Serial.println(goal);
	Serial.println(speed);
	Serial.println(accel);
	Serial.println(decel);
	Serial.println("fin params");
	if (decel > 0)
		decel = -decel;
	if (accel < 0)
		accel = -accel;
	if (speed < 0)
                speed = -speed;

        if (speed < current_speed)
            accel = decel;
        double time_acc = ((speed - current_speed) / accel);
        double d_acc = (abs(accel) * time_acc * time_acc / 2.0);
	double time_dec = (-speed / decel);
	double d_dec = (-decel * time_dec * time_dec / 2.0);
	double d_const = abs(goal - actue) - (d_acc + d_dec);
	
	Serial.print("time_acc");
	Serial.println(time_acc);
	Serial.print("d_acc");
	Serial.println(d_acc);
	Serial.print("time_dec");
	Serial.println(time_dec);
	Serial.print("d_dec");
	Serial.println(d_dec);
	Serial.print("d_const");
	Serial.println(d_const);

	// cas limite quand la vitesse max ne peut être atteinte
	if (d_const < 0)
	{
		Serial.println("oups");
		time_dec = sqrt(abs(goal - actue) / ((-decel / (2.0*accel) + 0.5) * -decel));
		speed = -decel * time_dec;
		d_acc = accel * time_acc * time_acc / 2.0;
		time_acc = speed / accel;
		d_acc = accel * time_acc * time_acc / 2.0;
		d_const = 0;
	}
	double time_const = (d_const / speed);
	Serial.print("time_const");
	Serial.println(time_const);

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
        _speed_actue = current_speed;
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
	
	Serial.print("_pos0");
	Serial.println(_pos0);
	Serial.print("_pos1");
	Serial.println(_pos1);
	Serial.print("_pos2");
	Serial.println(_pos2);
	Serial.print("_pos3");
	Serial.println(_pos3);
	Serial.println("rampe end");
}

void Rampe::compute_next_goal(long dt)
{
        _t += dt;

	switch (_phase)
	{
		case PHASE_ACCEL:
                {
			_acc_actue = _acc;

                        _speed_actue += _acc * dt;
                        /*if (_speed_actue > _speed) {
                            _speed_actue = _speed;
                        }*/

                        _pos_actue += _speed * dt;
                        /*if (_sens * _pos_actue > _sens * _pos1) {
                            _pos_actue = _pos1;
                        }*/

                        if (_t >= _t01) {
                            _phase = PHASE_CONST;
			}
			break;
		}
		case PHASE_CONST:
                {
			_acc_actue = 0;
                        _speed_actue = _speed;

                        _pos_actue +=_speed * dt;
                        if (_sens * _pos_actue > _sens * _pos2) {
                            _pos_actue = _pos2;
                        }

                        if (_t >= _t12){
                            _phase = PHASE_DECEL;
			}
			break;
		}
		case PHASE_DECEL:
                {
			_acc_actue = _dec;

                        _speed_actue += _dec * dt;
                        if (_speed_actue < 0) {
                            _speed_actue = 0;
                        }

                        _pos_actue += _speed * dt;
                        if (_sens * _pos_actue > _sens * _pos3) {
                            _pos_actue = _pos3;
                        }

                        if (_t >= _t23) {
                            _phase = PHASE_END;
			}
			break;
		}
		case PHASE_END:
		{
			_acc_actue = 0;
			_speed_actue = 0;
			_pos_actue = _pos3;
			break;
		}
                default: break;
	}
}

double Rampe::get_goal()
{
	return _pos_actue;
}

double Rampe::get_speed()
{
        return _speed_actue;
}


PHASE Rampe::get_phase()
{
	return _phase;
}



	
