#ifndef PID_H_
#define PID_H


#include <inttypes.h>

#define NB_ERRORS 128
	
class PID
{
	public:
		PID(double kp=0, double ki=0, double kd=0);
		double compute(double consign, double current);
		PID& setP(double p);
		PID& setI(double i);
		PID& setD(double d);
		PID& setGel(bool b);
		PID& reset();

	private:
		inline double computeErrSum(double err);
		double _kp, _ki, _kd;
		double _last;
		double _err_sum;
		int _i_tab_errs;
		double _tab_errs[NB_ERRORS];
		double _min, _max;
		bool _gel_active;
};


#endif

