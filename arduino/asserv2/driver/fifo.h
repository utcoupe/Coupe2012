#ifndef _FIFO_H
#define _FIFO_H


#define FIFO_SIZE	20
#define NB_DATAS	3

typedef enum {
	CMD_GOTO,
	CMD_TURN,
	CMD_GOTOR,
	CMD_TURNR,
	CMD_PWM,
	CMD_UNDEFINED
} T_FIFO_OBJ;


class FifoObj
{
	public:
		FifoObj();
		void set_t(T_FIFO_OBJ t);
		T_FIFO_OBJ get_t();
		int set_data(int i, int data);
		int get_data(int i);
		

	private:
		T_FIFO_OBJ _t;
		int _datas[NB_DATAS];
};


class Fifo
{
	public:
		Fifo();
		int push(T_FIFO_OBJ t, int data1=0, int data2=0, int data3=0);
		/* renvoie le prochain objet et change l'index de la FIFO */
		FifoObj * pop();
		/* renvoie le prochain objet mais ne change pas d'index */
		FifoObj * next();
		bool isEmpty();
		void clear();

	private:
		FifoObj _fifo[FIFO_SIZE];
		int _in,_out;
};

extern Fifo fifo;


#endif
