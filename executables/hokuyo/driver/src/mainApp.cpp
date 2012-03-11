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
 * Architecture
 * 		mainApp.cpp utilise
 * 		|- Le gestionnaire d'arguments (argManager.h/.cpp)
 * 		|- Le gestionnaire de com (comManager.h/.cpp)
 * 		|- Le gestionnaire de l'urg (urg*.cpp/.*)
 * */
 
// Inclusion Libs
#include "global.h"
#include "urgDriver.h"
#include "argManager.h"
#include "comManager.h"

// Déclaration des Managers
ArgManager* am;
ComManager* cm;
UrgDriver * ud;

// Fonctions d'interface 
#include "comThread.h"

/***********************************************************************
 * \fn int main(int argc, char *argv[])
 * 
 * <h1>Main</h1>
 * <p>Fonction d'entrée du programme</p>
 **********************************************************************/
int main(int argc, char *argv[])
{	
	//! --- Récupération des paramétres ---
	am = new ArgManager();
	am->addArg(new Argument<string>(TAG_PORTCOM));	// Le port com
	am->addArg(new Argument<int>(TAG_COLOR)); 		// La couleur 
	am->analyse(argc,argv);
	
	//! --- Start Hukuyo ---
	ud = UrgDriver::getUrgDriver();
	
	ud->setDelta(false,-40,-40);
	
		// On récupére le port com
	Argument<string>* portcom = am->getArg(TAG_PORTCOM,ArgManager::GET_STRING);
	if(portcom->isValid()){
		ud->setComPort(portcom->getValue());
	}
	else{
		ud->setComPort(ud->hokuyoFindPortCom(10));
	}
		// On récupére la couleur
	Argument<int>* argColor = am->getArg(TAG_COLOR,ArgManager::GET_INT);
	if(argColor->isValid()){
		ud->updateParamWithColor(argColor->getValue());
	}
	else{
		ud->updateParamWithColor(ud->hokuyoFindColor());
	}
	
	ud->refInit();
	ud->start();
	
	//! --- Start Com ---
	cm = ComManager::getComManager();
	cm->addKill(QH_KILL);
	cm->addFunction(QH_GETDATA,&send);
	cm->addFunction(QH_SET_REDCOLOR,&setRed);
	cm->addFunction(QH_SET_PURPLECOLOR,&setPurple);
	cm->setMutex(ud->getMutex());					// Partage des mutex
	cm->start();
    

	//! Ne pas oublier les wait, fruits de mon actuelle ignorance ^^
	ud->waitHere();    
	cm->waitHere();
	return 0;
}



