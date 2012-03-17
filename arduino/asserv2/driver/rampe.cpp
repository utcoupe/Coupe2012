#include "rampe.h"
#include "WProgram.h"
#include "wiring.h"


#include <math.h>



Rampe::Rampe()
{


}


void Rampe::compute(const double actue, const double goal,
                    const double c_goal_speed, const double c_accel, const double c_decel,
                    const double speed0, const double speedf)
{
	#define DEBUG_RAMPE
	#ifdef DEBUG_RAMPE
    Serial.println(__PRETTY_FUNCTION__);
    #endif


    // --------------------------------------------------------------------
    //              Correction des éventuelles erreurs
    // --------------------------------------------------------------------

    bool marche_avant = (actue < goal);
    double  goal_speed = c_goal_speed,
            accel = c_accel,
            decel = c_decel;

    // si marche avant
    //      vm > 0
    // si marche arrière
    //      vm < 0
    if (marche_avant)
    {
        goal_speed = abs(goal_speed);

        // v0 < vm
        //      acc > 0
        // v0 > vm
        //      acc = dec < 0
        if (speed0 < goal_speed)
            accel = abs(c_accel);
        else
            accel = -abs(c_decel);

        // vm > vf
        //      dec < 0
        // vm < vf
        //      acc = dec > 0
        if (goal_speed > speedf)
            decel = -abs(c_decel);
        else
            decel = abs(c_accel);
    }
    else
    {
        goal_speed = -abs(goal_speed);

        // v0 > vm
        //      acc < 0
        // v0 < vm
        //      acc = dec > 0
        if (speed0 > goal_speed)
            accel = -abs(c_accel);
        else
            accel = abs(c_decel);

        // vm < vf
        //      dec > 0
        // vm > vf
        //      acc = dec < 0
        if (goal_speed < speedf)
            decel = abs(c_decel);
        else
            decel = -abs(c_accel);
    }

	#ifdef DEBUG_RAMPE
    Serial.println("paramètres");
    Serial.print("actue: "); Serial.println(actue);
    Serial.print("goal: "); Serial.println(goal);
    Serial.print("goal_speed: "); Serial.println(goal_speed);
    Serial.print("accel: "); Serial.println(accel);
    Serial.print("decel: "); Serial.println(decel);
    Serial.print("speed0: "); Serial.println(speed0);
    Serial.print("speedf: "); Serial.println(speedf);
    Serial.println("end paramètres");
    #endif


    // --------------------------------------------------------------------
    //                  Assignement de variables
    // --------------------------------------------------------------------

    _no_decel = false;
    _no_accel = false;

    double max_speed = goal_speed;




    // --------------------------------------------------------------------
    //                  Premiers calculs
    //
    // dt = (vf - v0) / a
    // dx = a * dt² / 2 + v0 * dt
    // --------------------------------------------------------------------

    double time_acc = (goal_speed - speed0) / accel;
    double d_acc = (accel * time_acc * time_acc / 2.0) + speed0 * time_acc;
    double time_dec = (speedf - goal_speed) / decel;
    double d_dec = (decel * time_dec * time_dec / 2.0) + max_speed * time_dec;
    double d_const = (goal - actue) - (d_acc + d_dec);
    double time_const = d_const / max_speed;


    // --------------------------------------------------------------------
    //              Traitement des cas limites
    // --------------------------------------------------------------------

    // cas limite quand la vitesse max ne peut être atteinte
    if (abs(goal - actue) - (abs(d_acc) + abs(d_dec)) < 0)
    {
        time_const = 0;
        d_const = 0;
		#ifdef DEBUG_RAMPE
        Serial.println("oups");
        #endif
        // |v0| < |vm| && |vf| < |vm|
        // cas où la vitesse initiale est inférieur à la vitesse recherchée,
        // le profile sera un triangle
        //
        //   v1|v2
        //  /    \
        // v0     v3
        if (abs(speed0) < abs(max_speed) and abs(speedf) < abs(max_speed)) {
			#ifdef DEBUG_RAMPE
            Serial.println("triangle");
            #endif
            time_dec = sqrt(abs(goal - actue) / ((abs(decel) / (2.0*abs(accel)) + 0.5) * abs(decel)));
            max_speed = -decel * time_dec;
            d_acc = accel * time_acc * time_acc / 2.0;
            time_acc = max_speed / accel;
            d_acc = accel * time_acc * time_acc / 2.0;
        }
        // cas où la vitesse initiale est supérieure à la vitesse recherchée
        // le profile sera une décélération constante
        // Note: la décélération va être changée pour pouvoir atteindre l'objectif
        //
        // v0|v1|v2
        //         \
        //          \
        //          v3
        else {
			#ifdef DEBUG_RAMPE
            Serial.println("no accel");
            #endif
            _no_accel = true;
            time_acc = 0;
            d_acc = 0;
            decel = (speedf * speedf - speed0 * speed0) / (2.0 * (goal- actue));
            time_dec = (speedf - speed0) / decel;
            d_dec = (decel * time_dec * time_dec / 2.0) + speed0 * time_dec;
            max_speed = speed0;
        }
    }


    // --------------------------------------------------------------------
    //              Enregistrement de la rampe
    // --------------------------------------------------------------------

    // les goals
    _goal = goal;
    _goal_speed = goal_speed;
    _speedf = speedf;
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
    _pos1 = _pos0 + d_acc;
    _pos2 = _pos1 + d_const;
    _pos3 = goal;
    _phase = PHASE_ACCEL;

	#ifdef DEBUG_RAMPE
    Serial.println("times");
    Serial.println(_t1);
    Serial.println(_t2);
    Serial.println(_t3);
    Serial.println("end times");

    Serial.println("pos");
    Serial.println(_pos1);
    Serial.println(_pos2);
    Serial.println(_pos3);
    Serial.println("end pos");

    Serial.println(_speedf);

    Serial.print("end ");
    Serial.println(__PRETTY_FUNCTION__);
    #endif

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
                        if (_no_decel)
                            _current_speed = _max_speed;
                        else
                            _current_speed = _speedf;
                        _current_pos = _pos3;
			break;
		}
                default: break;
	}
}

double Rampe::get_pos()
{
        return _current_pos;
}

double Rampe::get_speed()
{
        return _current_speed;
}

double Rampe::get_aspeed()
{
    return abs(Rampe::get_speed());
}


PHASE Rampe::get_phase()
{
        return _phase;
}

void Rampe::update_speedf(double speedf)
{
    double t = _t;
    Rampe::compute(_current_pos, _goal, _goal_speed, _acc, _dec, _current_speed, speedf);
    _t = t;
}

	
