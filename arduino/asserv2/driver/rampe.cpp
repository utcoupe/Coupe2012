#include "rampe.h"
#include "WProgram.h"
#include <math.h>



Rampe::Rampe()
{


}


void Rampe::compute(double actue, double goal, double max_speed, double accel, double decel, double speed0)
{
    this->_no_decel = false;

	Serial.println("rampe");
	Serial.println(actue);
	Serial.println(goal);
        Serial.println(max_speed);
	Serial.println(accel);
	Serial.println(decel);
	Serial.println("fin params");

        if (actue < goal) {
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
        }
        else {
            if (decel < 0)
                decel = -decel;
            if (accel > 0)
                accel = -accel;
            if (max_speed > 0)
                max_speed = -max_speed;
            if (speed0 > 0)
                speed0 = -speed0;

            if (max_speed > speed0)
                accel = decel;
        }




        double time_acc = ((max_speed - speed0) / accel);
        double d_acc = (accel * time_acc * time_acc / 2.0) + speed0 * time_acc;
        double time_dec = (max_speed / -decel);
        double d_dec = (decel * time_dec * time_dec / 2.0) + max_speed * time_dec;
        double d_const = (goal - actue) - (d_acc + d_dec);
	
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
        if (abs(goal - actue) - (abs(d_acc) + abs(d_dec)) < 0)
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
                    this->_no_accel = true;
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

        /*if (goal > actue)
            this->_sens = 1;
        else
            this->_sens = -1;*/

        // les goals
        this->_goal = goal;
        this->_max_speed = max_speed;
        this->_acc = accel;
        this->_dec = decel;
        // les valeurs actueelles théoriques
        this->_current_pos = actue;
        this->_current_speed = speed0;
        this->_acc_actue = 0;
        this->_t = 0;
	// temps de chaque début de phase
        this->_t1 = time_acc;
        this->_t2 = this->_t1 + time_const;
        this->_t3 = this->_t2 + time_dec;
	// position de chaque début de phase
        this->_pos0 = actue;
        this->_pos1 = this->_pos0 + d_acc;
        this->_pos2 = this->_pos1 + d_const;
        this->_pos3 = goal;
        this->_phase = PHASE_ACCEL;
	
	Serial.print("_pos0");
        Serial.println(this->_pos0);
	Serial.print("_pos1");
        Serial.println(this->_pos1);
	Serial.print("_pos2");
        Serial.println(this->_pos2);
	Serial.print("_pos3");
        Serial.println(this->_pos3);
	Serial.println("rampe end");
}

void Rampe::compute_next_goal(double dt)
{
        this->_t += dt;

        switch (this->_phase)
	{
                case PHASE_ACCEL:   // pos0 -> pos1
                {
                        this->_acc_actue = this->_acc;

                        if (this->_t < this->_t1) {
                            this->_current_speed += this->_acc * dt;
                            this->_current_pos += this->_current_speed * dt;
                            break;
			}
                        else {
                            this->_phase = PHASE_CONST;
                            this->_current_pos = this->_pos1;
                            dt = (this->_t - this->_t1);
                        }

		}
                case PHASE_CONST:   // pos1 -> pos2
                {
                        this->_acc_actue = 0;
                        this->_current_speed = this->_max_speed;

                        if (this->_t < this->_t2){
                            this->_current_pos += this->_current_speed * dt;
                            break;
                        }
                        else {
                            this->_phase = PHASE_DECEL;
                            dt = (this->_t - this->_t2);
                            this->_current_pos = this->_pos2;
                        }
		}
                case PHASE_DECEL:   // pos2 -> pos3
                {
                        this->_acc_actue = this->_dec;

                        this->_current_speed += this->_dec * dt;

                        this->_current_pos += this->_current_speed * dt;

                        if (this->_t < this->_t3) {
                            break;
                        }
                        else {
                            this->_phase = PHASE_END;
                            this->_current_pos = this->_pos3;
                        }
		}
		case PHASE_END:
		{
                        this->_acc_actue = 0;
                        if (!_no_decel)
                            this->_current_speed = 0;
                        else
                            this->_current_speed = this->_max_speed;
                        this->_current_pos = this->_pos3;
			break;
		}
                default: break;
	}
}

double Rampe::get_goal()
{
        return this->_current_pos;
}

double Rampe::get_speed()
{
        return this->_current_speed;
}


PHASE Rampe::get_phase()
{
        return this->_phase;
}

void Rampe::cancel_decel()
{
    if (this->_no_accel) {
        //@todo
    }
    else {
        // plus de décélération
        this->_no_decel = true;
        // augmentation de la durée de la phase constante
        this->_t2 += (this->_pos3 - this->_pos2) / this->_max_speed;
        // la position finale de la phase constante devient la position finale
        this->_pos2 = this->_pos3;
        // durée phase constante = 0;
        this->_t3 = this->_t2;
    }
}

	
