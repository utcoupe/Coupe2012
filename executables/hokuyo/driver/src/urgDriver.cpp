/**
 * \file 	urgDriver.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	10/03/2012
 *
 * Fontions de gestion du threading 
 * */
 

#include "urgDriver.h"
#include "urgException.h" 
	
using namespace std;
	
//! Initialisation du singleton
UrgDriver* UrgDriver::driverObj = 0;

/***********************************************************************
 * <h1>Constructeur</h1>
 **********************************************************************/
UrgDriver::UrgDriver()
{
	pthread_mutex_init(&(this->mutex), NULL);
}

/***********************************************************************
 * <h1>Destructeur</h1>
 **********************************************************************/
UrgDriver::~UrgDriver()
{
	deconnectHokuyo();
	delete[] distanceMax;
}

/***********************************************************************
 * <p>Création du message</p>
 **********************************************************************/
void UrgDriver::sendInfos()
{
	bool pass=false;
	list<coord>::iterator it;
	for ( it=robot.begin() ; it!=robot.end() ; it++ )
	{
		if(pass){
			cout << ","; 
		}
		else{
			pass=true;
		}
		cout << "(" << (*it).x << "," << (*it).y << ")";
	}
}

/***********************************************************************
 * \return UrgDriver* - pointeur sur le singleton du driver
 * <p>Fonction pour récupérer le singleton</p>
 **********************************************************************/
UrgDriver* UrgDriver::getUrgDriver()
{
	if(driverObj == 0) {
		driverObj = new UrgDriver();
		return driverObj;
	}
	else{
		return driverObj;
	}		
}

/***********************************************************************
 * <p>Démarre le processus de controle de la com</p>
 **********************************************************************/
void UrgDriver::start()
{
	askValue=true;;
	if(pthread_create(&thr, NULL, &UrgDriver::helpfct, NULL)) {
		cerr << "Erreur : Impossible de créer le Thread Urg" << endl;
		throw new urgException(this, urgException::Err_start_threadPb);
    }
    
}

void UrgDriver::waitHere()
{
	if(pthread_join(thr, NULL)) {
		cerr << "Erreur : Impossible de joindre le Thread Urg" << endl;;
    }
}

void UrgDriver::stop()
{
	askValue=false;
}

/***********************************************************************
 * <p>Fonction qui sera la tâche du thread urg. Son comportement peut 
 * étre définit dans la fonction UrgDriver::loop</p>
 **********************************************************************/ 
void* UrgDriver::helpfct(void* arg)
{
	void* ret = NULL;
	try {
		ret = getUrgDriver()->loop(arg);
	}
	catch(urgException* e){
		e->react();
	}
	return ret;
}

/***********************************************************************
 * 
 **********************************************************************/
void* UrgDriver::loop(void* arg)
{	
	
	if(!urg.isConnected()) {
		throw new urgException(this, urgException::Err_loop_urgNoConnect);
		return NULL;
	}
	
	
		
	long n;
	while(askValue)
	{
		long timestamp = 0;
		std::vector<long> data;
		n=getData(data,&timestamp);
		
		if(n>0){
			// C'est ici que l'on traite les données
			interpretData(data,n);	
		}	
	}

	return NULL;
}
