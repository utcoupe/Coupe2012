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
	
void UrgDriver::toString() 
{
	cout << "--- URG DRIVER CONFIG ---" << endl << endl;
	if(color==ROUGE) {
		cout << "Color ROUGE"<< endl;
	}
	else {
		cout << "Color VIOLET"<< endl;
	}
	
	cout << "Com Port" << comPort << endl;	

	cout << "radMin" << radMin << endl;
	cout << "radMax" << radMax << endl;	

/*
	for(int i=indexMin ; i<indexMax ; i++) 
	{
		cout << "[" << urg.index2deg(i) << ";" << distanceMax[i] << "]" << "__";
	}
*/

}
	
//! Initialisation du singleton
UrgDriver* UrgDriver::driverObj = 0;

/**
 * <h1>Constructeur</h1>
 * */
UrgDriver::UrgDriver()
{
	this->setDelta(false);
	pthread_mutex_init(&(this->mutex), NULL);
}

/**
 * <h1>Destructeur</h1>
 * */
UrgDriver::~UrgDriver()
{
	deconnectHokuyo();
	delete[] distanceMax;
}

/**
 * <p>Création du message</p>
 * */
void UrgDriver::sendInfos()
{
	cout << "[";
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
	cout << "]";
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

/**
 * <p>Démarre le processus de controle de la com</p>
 * */
void UrgDriver::start()
{
	// 
	askValue=true;
	
	// Création du thread
	if(pthread_create(&thr, NULL, &UrgDriver::helpfct, NULL)) {
		throw new urgException(this, urgException::Err_start_threadPb);
    }
}

/**
 * 
 * */
void UrgDriver::waitHere()
{
	// TODO 
	// Gérer l'erreur par exception
	if(pthread_join(thr, NULL)) {
		cerr << "Erreur : Impossible de joindre le Thread Urg" << endl;;
    }
}

/**
 * <h2>Stop</h2>
 * <p>Arrét du thread</p>
 * */
void UrgDriver::stop()
{
	askValue=false;
}

/**
 * <p>Fonction d'interface pour la gestion du thread. 
 * Son comportement peut étre définit dans la fonction 
 * UrgDriver::loop</p>
 * */ 
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

/**
 * <h2>Loop</h2>
 * <p>Fonction principale de récupération et d'analyse des données</p>
 * */
void* UrgDriver::loop(void* arg)
{	
	// On test la connection
	if(!urg.isConnected()) {
		throw new urgException(this, urgException::Err_loop_urgNoConnect);
		return NULL;
	}
		
	// Boucle de traitement
	long n;
	while(askValue)
	{
		long timestamp = 0;
		std::vector<long> data;
		n=getData(data,&timestamp);
		
		// C'est ici que l'on traite les données
		interpretData(data,n);	
	}

	return NULL;
}

//! ---------- END OF FILE ----------
