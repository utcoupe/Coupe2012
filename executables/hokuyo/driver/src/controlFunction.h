/**
 * \file 	controlFunction.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * */
#ifndef CONTROLFUNCTION_H
#define CONTROLFUNCTION_H

//! ---
inline bool startHokuyoDriver();
inline bool driverInitialisation();
inline bool driverStart();
inline bool driverClean();
void hokLoop();
void stopHokLoop();
//! ---




//!
inline
bool driverStart()
{
	// Start Com
    comLoop();
	
	// Loop hok
	hokLoop();
 
	return true;
}


//! Ensemble de la structure de l'application
inline 
bool startHokuyoDriver()
{
	#if DEBUG
	cout << "Demarrage du driver hokuyo" << endl << endl;
	#endif


	// Initialisation
	if(!driverInitialisation()){
		cout << "Erreur :driverInitialisation()" << endl;
		return false;
	}
	
	
	// Demarrage des fonctions princpales
	if(!driverStart()){
		cout << "Erreur :driverStart()" << endl;
		return false;
	}
	
	// Netoyage des variables
	if(!driverClean()){
		cout << "Erreur :driverClean()" << endl;
		return false;
	}
	
	#if DEBUG
	cout  << endl << "Arrét du driver hokuyo" << endl;
	#endif	
	
	return true;
}





//!
inline
bool driverInitialisation()
{
	
	// --- Récupérer le port com ---
	if(!appParameter.comPort.compare("")){
		g_comPort = hokuyoFindPortCom();
	}else{
		g_comPort = appParameter.comPort;
	}
	
	// --- Récupérer notre couleur ---
	if(appParameter.color == -1){
		g_color = hokuyoFindColor();
	}else{
		g_color = appParameter.color;
	}
	
	hokuyoSetGetParam();
	
	// --- On démarre l'hokuyo
	if(!startHokuyo()) {
		cout << "Erreur :controlFunction:startHokuyo" << endl;
		return false;
	}
	
	// --- Initialisation des références
	if(!hokuyoReference()) {
		cout << "Erreur :controlFunction:hokuyoReference" << endl;
		return false;
	}

	return true;
}




//!
inline
bool driverClean()
{
	stopHokuyo();

	delete[] g_distanceMax;

	return true;
}



//!
void hokLoop()
{
	pthread_t thrHok;
    if(pthread_create(&thrHok, NULL, &urgLoop, NULL))
    {
        printf("Could not create thread hok\n");
        return -1;
    }
    
    if(pthread_join(thrHok, NULL))
    {
        printf("Could not join thread\n");
        return -1;
    }
}

//!
void stopHokLoop()
{
	g_stop = true;
}


#endif // CONTROLFUNCTION_H
