/**
 * \file 	urgDriver.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	10/03/2012
 *
 * Fontions de gestion du threading 
 * */
 

#include "urgDriver.h"
	
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

//! --- 
void UrgDriver::start()
{
	askValue=true;;
	if(pthread_create(&thr, NULL, &UrgDriver::helpfct, NULL)) {
		cerr << "Erreur : Impossible de créer le Thread Urg" << endl;
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

//! --- 
void* UrgDriver::helpfct(void* arg)
{
	return getUrgDriver()->loop(arg);
}

//! --- 
void* UrgDriver::loop(void* arg)
{	
	if(!urg.isConnected()) {
		cerr << "Erreur Urg non connecté" << endl;
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
