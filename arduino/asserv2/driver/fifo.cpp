#include "fifo.h"

#include "WProgram.h"
#include "wiring.h"




Fifo fifo;


Fifo::Fifo()
{
	_in = 0;
	_out = 0;
}


int Fifo::push(T_FIFO_OBJ t, int data1, int data2, int data3)
{
	_fifo[_in].set_t(t);
	_fifo[_in].set_data(0,data1);
	_fifo[_in].set_data(1,data2);
	_fifo[_in].set_data(2,data3);
	
	_in++;
	if (_in >= FIFO_SIZE)
		_in = 0;
		
	return 0;
}


FifoObj * Fifo::pop()
{
	if (Fifo::isEmpty())
		return NULL;
	else
	{
		FifoObj * obj = &(_fifo[_out]);
		
		_out++;
		if (_out >= FIFO_SIZE)
			_out = 0;
		
		return obj;
	}
}

FifoObj * Fifo::next()
{
	if (Fifo::isEmpty())
		return NULL;
	else
	{
		return &(_fifo[_out]);
	}
}


bool Fifo::isEmpty()
{
	return _in == _out;
}


void Fifo::clear()
{
	_in = _out;
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


