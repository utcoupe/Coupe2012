/**
 * \file 	sample.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	01/04/2012          // et c'est pas une blague !!!	
 * 
 * */

#include "argManager.h"

int main(int argc, char *argv[]) 
{
	
	// singleton 
	ArgManager* am = new ArgManager();
	
	// Ajout des paramétres 
	am->addArg(new Argument<string>("-tag"));	// Le port com
	
	// Analyse
	am->analyse(argc,argv);
	
	// 
	Argument<string>* portcom = am->getArg("-tag",ArgManager::GET_STRING);
	if(portcom->isValid()){
			cout << "-tag = " <<  portcom->getValue() << endl;
	}
	else{
			cout << "le tag n'a pas été trouvé" << endl;
	}
	
	return 0;
}
