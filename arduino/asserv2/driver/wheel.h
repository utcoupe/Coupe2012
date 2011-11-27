


class Wheel
{
	public:
		Wheel();
		void go_to(long pos);

	private:
		long _goal;
		PID _pid;
};


