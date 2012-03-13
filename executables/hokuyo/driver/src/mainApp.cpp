/**
 * \file 	mainApp.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	29/02/2012
 * 
 * 
 * Architecture
 * 		mainApp.cpp utilise
 * 		|- Le gestionnaire d'arguments (argManager.h/.cpp)
 * 		|- Le gestionnaire de com (comManager.h/.cpp)
 * 		|- Le gestionnaire de l'urg (urg*.cpp/.*)
 * */
 
// Inclusion Libs
#include "global.h"

#include "mainAppDriver.h" 


//! Instance global
MainAppDriver* application;

// Exception qui ont besoin parfois d'avoir un accés
#include "urgException.h"




/***********************************************************************
 * \fn int main(int argc, char *argv[])
 * 
 * <h1>Main</h1>
 * <p>Fonction d'entrée du programme</p>
 **********************************************************************/
int main(int argc, char *argv[])
{	
	
		application = new MainAppDriver(argc,argv);
		application->waitHere();
	
	
	return 0;
}





// Fonctions d'interface 
#include "comThread.h"  // ça c'est ici parce que les fonctions ont besoin
						// l'instance global application
						// Je bougerai ça après

/***********************************************************************
 * 
 **********************************************************************/
void MainAppDriver::initComManager()
{
	//! --- Start Com ---
	cm = ComManager::getComManager();
	cm->addKill(QH_KILL);
	cm->addFunction(QH_GETDATA,&send);
	cm->addFunction(QH_SET_REDCOLOR,&setRed);
	cm->addFunction(QH_SET_PURPLECOLOR,&setPurple);
	cm->setMutex(ud->getMutex());					// Partage des mutex
	cm->start();
}
