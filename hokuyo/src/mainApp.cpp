/**
 * Application de gestion de l'hokuyo
 * 
 * */
#define DEBUG 1
#define NB_MAX_ROBOT 4
#define URG_AUTO_CAPTURE 1

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <pthread.h>

#include "delay.h"

// Définition Globale, la façon la plus simple de faire communiquer des
// threads ^^
struct coord{
	int x;
	int y;
};

bool stop=false;
pthread_mutex_t mutex;
coord robot[NB_MAX_ROBOT];

#include "checkParameters.h"
#include "protocoleCom.h"
#include "urgFile.h"


//! --- MAIN ^^ ---
int main(int argc, char *argv[])
{
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
	
	// Kill des threads
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

	return 0;
}
