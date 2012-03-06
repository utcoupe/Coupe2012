/**
 * \file 	comThread.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * */
#ifndef COMTHREAD_H
#define COMTHREAD_H

#include "comManager.h"


/**
 * Fonction retournant la postion des robots en coordonnées cartésiennes  
 * */ 
#define QH_GETDATA 1 
void send(void)
{
	pthread_mutex_lock(&mutex);
	bool pass=false;
	std::list<coord>::iterator it;
	for ( it=robot.begin() ; it!=robot.end() ; it++ )
	{
		if(pass){
			cout << ","; 
		}
		cout << "(" << (*it).x << "," << (*it).y << ")";
		pass=true;
	}
	pthread_mutex_unlock(&mutex);
}


/**
 * Signal pour stoper l'application hokuyo
 * */ 
#define QH_KILL 9
// No Function for kill






void* comLoop(void* arg)
{
	// Définition des foncions de la com en commançant par la commande kill
	comManager cm(QH_KILL);	
	cm.addFunction(QH_GETDATA,&send);
	
	void* ret = cm.loop(arg);
	g_stop = true;
	return ret;
}

#endif // COMTHREAD_H
