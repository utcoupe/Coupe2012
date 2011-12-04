#ifndef _PID
#define _PID

#define NB_ERRORS	64

class PID
{
	public:
		PID(double kp, double ki, double kd);
		void setCoeffs(double kp, double ki, double kd);
		void setOutputLimits(int a, int b);
		int compute(int error);
		void reset();
	
	private:
		void addErr(double error);
		void resetErrors();

		int _errors[NB_ERRORS];
		long int _errSum;
		int _iErr;
		int _lastError;
		double _kp, _ki, _kd;
		int _output_lim_a, _output_lim_b;
};




#endif


