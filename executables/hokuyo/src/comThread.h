/**
 * \file 	comThread.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * */
#ifndef COMTHREAD_H
#define COMTHREAD_H

#include "comManager.h"

void* comLoop(void* arg)
{
	
	
	comManager cm;
	void* ret = cm.loop(arg);
	g_stop = true;
	return ret;
}

#endif // COMTHREAD_H
