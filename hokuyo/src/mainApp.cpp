/**
 * \file 	mainApp.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	29/02/2012
 * 
 * TODO
 * - Traiter les données incohérentes
 * 		-> nbRobot > 4
 * 		-> pas de données sur les robots présents sur la table 
 * 
 * -Caler l'angle sur la balise
 * -ignorer la balise de départ dans le calcul des robots
 * -Ajuster le (0,0) réel de l'hokuyo avec le théorique
 * 
 * */
 
#include "global.h"
#include "checkParameters.h"
#include "protocoleCom.h"
#include "urgFile.h"
#include "urgStartFunction.h"

//! --- MAIN ^^ ---
int main(int argc, char *argv[])
{	
	/*
	// Check appliation parameters
	MainParameters mParameters; 
	checkParameters(&mParameters,argc,argv);
	*/
	
	initHokuyo();
	
	

	// Création thread
	pthread_mutex_init(&mutex, NULL);	

	pthread_t thrCom;
    if(pthread_create(&thrCom, NULL, &comLoop, NULL))
    {
        printf("Could not create thread com\n");
        return -1;
    }
	pthread_t thrHok;
    if(pthread_create(&thrHok, NULL, &urgAnalyse, NULL))
    {
        printf("Could not create thread hok\n");
        return -1;
    }
	
	// Kill propre des threads
    if(pthread_join(thrCom, NULL))
    {
        printf("Could not join thread\n");
        return -1;
    }
    if(pthread_join(thrHok, NULL))
    {
        printf("Could not join thread\n");
        return -1;
    }

	delete[] g_distanceMax;
 
	return 0;
}
