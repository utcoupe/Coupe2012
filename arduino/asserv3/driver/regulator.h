#ifndef REGULATEUR_H_
#define REGULATEUR_H_

#include "pid.h"
#include <inttypes.h>

class Regulator
{
	public:
		Regulator();

		/**
		 * Calculer la sortie de commande
		 * @param consign la valeur voulue
		 * @param current la valeur actuelle
		 */
		double compute(double consign, double current);

		Regulator& setPID(double,double,double);
		Regulator& setGel(bool);

		/**
		 * Saturer la sortie entre [-l,l]
		 * Si l<0 alors pas de limite.
		 * @param l
		 */
		Regulator& setOutLimit(double l);
		
		Regulator& reset();

	private:
		double _out_limit;			// limite de sortie
		PID _pid;
};

class PosAndSpeedRegulator
{
	public:
		typedef enum {BOTH, POSITION, SPEED} T_MOD;
		PosAndSpeedRegulator();
		double compute(double consign, double current_pos, double current_speed);

		PosAndSpeedRegulator& setPosPID(double,double,double);
		PosAndSpeedRegulator& setSpeedPID(double,double,double);
		PosAndSpeedRegulator& setSpeedLimit(double);
		PosAndSpeedRegulator& setAccelLimit(double);
		PosAndSpeedRegulator& setOutLimit(double);
		PosAndSpeedRegulator& setMod(PosAndSpeedRegulator::T_MOD);
		PosAndSpeedRegulator& setGel(bool);

		PosAndSpeedRegulator& stop();
		PosAndSpeedRegulator& resume();
		PosAndSpeedRegulator& reset();

		double getSpeedConsign() const;
		
	private:
		bool _on;
		double _accel_limit, _last_speed_consign, _out_limit;
		Regulator _pos_regu, _speed_regu;
		T_MOD _mod;

		
		double _bothCompute(double consign, double current_pos, double current_speed);
		double _speedCompute(double consign, double current_speed);
		double _positionCompute(double consign, double current_pos);
};


#endif


