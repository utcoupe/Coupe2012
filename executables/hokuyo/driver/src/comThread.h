/**
 * \file 	comThread.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * */
#ifndef COMTHREAD_H
#define COMTHREAD_H

#include "../protocole.h"
#include "comManager.h"

/**
 * Fonction retournant la postion des robots en coordonnées cartésiennes  
 * */ 
// #define QH_GETDATA 1 
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
// #define QH_KILL 9
// No Function for kill






void comLoop()
{
	// On récupére le singleton du manager
	comManager* cm = comManager::getComManager();
	
	// Partage du mutex
	mutex = cm->getMutex();
	
	// Ajout des fonctions 
	cm->addKill(QH_KILL);
	cm->addFunction(QH_GETDATA,&send);
	
	// Démarrage du thread de com 
	cm->start();

	g_stop = true;	
}



#endif // COMTHREAD_H
