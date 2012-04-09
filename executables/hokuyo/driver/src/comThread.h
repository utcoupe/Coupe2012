/**
 * \file 	comThread.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * <p>Fonctions à lier aux define dans le comManager</p>
 * 
 * */
#ifndef COMTHREAD_H
#define COMTHREAD_H

#include "protocole.h"

/***********************************************************************
 * <h1>QH_GETDATA</h1>
 * <p>Fonction retournant la postion des robots en coordonnées 
 * cartésiennes</p>  
 **********************************************************************/ 
// #define QH_GETDATA 1 
void send(void)
{
	application->ud->sendInfos();
}

/***********************************************************************
 * <h1>QH_SET_REDCOLOR</h1>
 * <p>Dit à l'hokuyo que notre robot est sur la couleur rouge</p>  
 **********************************************************************/ 
// #define QH_SET_REDCOLOR 2 
void setRed(void)
{
	application->ud->stop();
	application->ud->updateParamWithColor(ROUGE);
	application->ud->start();
}

/***********************************************************************
 * <h1>QH_SET_PURPLECOLOR</h1>
 * <p>Dit à l'hokuyo que notre robot est sur la couleur violet</p>  
 **********************************************************************/
// #define QH_SET_PURPLECOLOR 3
void setPurple(void)
{
	application->ud->stop();
	application->ud->updateParamWithColor(BLEU);
	application->ud->start();
}

/***********************************************************************
 * <h1>QH_KILL</h1>
 * <p>Kill com</p>  
 **********************************************************************/
// #define QH_KILL 9
// No Function for kill


#endif // COMTHREAD_H
