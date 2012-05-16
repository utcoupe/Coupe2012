#ifndef __TAPIS_H__
#define __TAPIS_H__



#define PIN_TAPIS		42


class Tapis {

	public:
		Tapis();
		int actionner(int on);
};


extern Tapis g_tapis;


#endif



