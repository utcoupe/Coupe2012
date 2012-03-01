/**
 * \file 	mainApp.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	29/02/2012
 * 
 * TODO
 * - Réorganiser le code 
 * - Calibrage auto
 * - Choix de la couleur
 * 
 * */
 
#include "global.h"
#include "checkParameters.h"
#include "protocoleCom.h"
#include "urgFile.h"

//! --- MAIN ^^ ---
int main(int argc, char *argv[])
{	
	
	string ahahah = getTtyAcm();
	
	// Check appliation parameters
	MainParameters mParameters; 
	checkParameters(&mParameters,argc,argv);

	// Création thread
	pthread_mutex_init(&mutex, NULL);	

	pthread_t thrCom;
    if(pthread_create(&thrCom, NULL, &comLoop, NULL))
    {
        printf("Could not create thread com\n");
        return -1;
    }
	pthread_t thrHok;
    if(pthread_create(&thrHok, NULL, &urgAnalyse, (void*)&mParameters))
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

	delete[] distanceMax;
	return 0;
}
