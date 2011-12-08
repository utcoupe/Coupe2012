#include "rampe.h"
#include "WProgram.h"
#include <math.h>



Rampe::Rampe()
{


}


void Rampe::compute(double actue, double goal, double max_speed, double accel, double decel, double speed0)
{
	Serial.println("rampe");
	Serial.println(actue);
	Serial.println(goal);
        Serial.println(max_speed);
	Serial.println(accel);
	Serial.println(decel);
	Serial.println("fin params");
	if (decel > 0)
		decel = -decel;
	if (accel < 0)
		accel = -accel;
        if (max_speed < 0)
                max_speed = -max_speed;
        if (speed0 < 0)
                speed0 = -speed0;

        if (max_speed < speed0)
            accel = decel;
        double time_acc = ((max_speed - speed0) / accel);
        double d_acc = (accel * time_acc * time_acc / 2.0) + speed0 * time_acc;
        double time_dec = (max_speed / -decel);
        double d_dec = (decel * time_dec * time_dec / 2.0) + max_speed * time_dec;
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
                if (speed0 < max_speed) {
                    time_dec = sqrt(abs(goal - actue) / ((-decel / (2.0*accel) + 0.5) * -decel));
                    max_speed = -decel * time_dec;
                    d_acc = accel * time_acc * time_acc / 2.0;
                    time_acc = max_speed / accel;
                    d_acc = accel * time_acc * time_acc / 2.0;
                    d_const = 0;
                }
                else {  // décélération continue
                    time_acc = 0;
                    d_acc = 0;
                    d_const = 0;
                    time_dec = (speed0 / -decel);
                    d_dec = (decel * time_dec * time_dec / 2.0) + speed0 * time_dec;
                    if (d_dec > abs(actue - goal)) {    // pas le temps de ralentir !
                        decel = -speed0 * speed0 / (2.0 * (goal- actue));
                        time_dec = (speed0 / -decel);
                        d_dec = (decel * time_dec * time_dec / 2.0) + speed0 * time_dec;
                    }
                    max_speed = speed0;
                }
        }
        double time_const = (d_const / max_speed);
        Serial.print("time_const");
	Serial.println(time_const);

	if (goal > actue)
		_sens = 1;
	else
		_sens = -1;
	
	// les goals
	_goal = goal;
        _max_speed = max_speed;
	_acc = accel;
	_dec = decel;
	// les valeurs actueelles théoriques
        _current_pos = actue;
        _current_speed = speed0;
	_acc_actue = 0;
	_t = 0;
	// temps de chaque début de phase
        _t1 = time_acc;
        _t2 = _t1 + time_const;
        _t3 = _t2 + time_dec;
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

void Rampe::compute_next_goal(double dt)
{
        _t += dt;

	switch (_phase)
	{
                case PHASE_ACCEL:   // pos0 -> pos1
                {
			_acc_actue = _acc;

                        if (_t < _t1) {
                            _current_speed += _acc * dt;
                            _current_pos += _current_speed * dt;
                            break;
			}
                        else {
                            _phase = PHASE_CONST;
                            _current_pos = _pos1;
                            dt = (_t - _t1);
                        }

		}
                case PHASE_CONST:   // pos1 -> pos2
                {
			_acc_actue = 0;
                        _current_speed = _max_speed;

                        if (_t < _t2){
                            _current_pos += _current_speed * dt;
                            break;
                        }
                        else {
                            _phase = PHASE_DECEL;
                            dt = (_t - _t2);
                            _current_pos = _pos2;
                        }
		}
                case PHASE_DECEL:   // pos2 -> pos3
                {
			_acc_actue = _dec;

                        _current_speed += _dec * dt;

                        _current_pos += _current_speed * dt;

                        if (_t < _t3) {
                            break;
                        }
                        else {
                            _phase = PHASE_END;
                            _current_pos = _pos3;
                        }
		}
		case PHASE_END:
		{
			_acc_actue = 0;
                        _current_speed = 0;
                        _current_pos = _pos3;
			break;
		}
                default: break;
	}
}

double Rampe::get_goal()
{
        return _current_pos;
}

double Rampe::get_speed()
{
        return _current_speed;
}


PHASE Rampe::get_phase()
{
	return _phase;
}

void Rampe::cancel_decel()
{
    // @TODO
}

	
