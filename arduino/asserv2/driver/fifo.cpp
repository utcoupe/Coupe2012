#include "fifo.h"

#include "WProgram.h"


Fifo fifo;


Fifo::Fifo()
{
	_in = 0;
	_out = 0;
}


int Fifo::push(T_FIFO_OBJ t, int data1, int data2, int data3)
{
	_in++;
	if (_out == _in)
	{
		char msg[50];
		sprintf(msg, "Error -- %s (%s:%d) -- Fifo is empty", __FUNCTION__, __FILE__, __LINE__);
		Serial.println(msg);
		
		_in--;
		return -1;
	}
	else
	{
		_fifo[_in].set_t(t);
		_fifo[_in].set_data(0,data1);
		_fifo[_in].set_data(1,data2);
		_fifo[_in].set_data(2,data3);
		
		return 0;
	}
}


FifoObj * Fifo::pop()
{
	if (Fifo::isEmpty())
		return NULL;
	else
	{
		_out++;
		
		return &(_fifo[_out-1]);
	}
}


bool Fifo::isEmpty()
{
	return _in == _out;
}





FifoObj::FifoObj()
{
	_t = CMD_UNDEFINED;
	for(int i=0; i<NB_DATAS; ++i)
		_datas[i] = 0;
}



void FifoObj::set_t(T_FIFO_OBJ t)
{
	_t = t;
}

T_FIFO_OBJ FifoObj::get_t()
{
	return _t;
}

int FifoObj::set_data(int i, int data)
{
	if (i >= NB_DATAS or i < 0)
	{
		char msg[50];
		sprintf(msg, "Error -- %s (%s:%d) -- Invalid index : (%d)", __FUNCTION__, __FILE__, __LINE__, i);
		Serial.println(msg);

		return -1;
	}
	else
	{
		_datas[i] = data;

		return 0;
	}
}

int FifoObj::get_data(int i)
{
	if (i >= NB_DATAS or i < 0)
	{
		char msg[50];
		sprintf(msg, "Error -- %s (%s:%d) -- Invalid index : (%d)", __FUNCTION__, __FILE__, __LINE__, i);
		Serial.println(msg);

		return 0;
	}
	else
	{
		return _datas[i];
	}
}

