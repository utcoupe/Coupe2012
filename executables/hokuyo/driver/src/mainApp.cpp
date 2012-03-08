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
#include "urgThread.h"
#include "comThread.h"
#include "checkParameters.h"
#include "urgManagementFunction.h"
#include "controlFunction.h"

//! --- MAIN ^^ ---
int main(int argc, char *argv[])
{	
	//
	checkParameters(&appParameter, argc, argv);
	
	//
	if(!startHokuyoDriver()){
		return -1;
	}
	
	//
	return 0;
}



