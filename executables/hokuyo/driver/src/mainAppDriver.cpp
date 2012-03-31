


#include "mainAppDriver.h"



/***********************************************************************
 * <h1>Constructeur</h1>
 * 
 * @param arg - parametres du main à passer
 **********************************************************************/
MainAppDriver::MainAppDriver(int argc, char *argv[])
{
	
	initArgManager(argc,argv);
	initUrgDriver();
	initComManager();
	
	
	
}

/***********************************************************************
 * 
 **********************************************************************/
void MainAppDriver::initArgManager(int argc, char *argv[])
{
	//! --- Récupération des paramétres ---
	am = new ArgManager();
	am->addArg(new Argument<string>(TAG_PORTCOM));	// Le port com
	am->addArg(new Argument<int>(TAG_COLOR)); 		// La couleur 
	am->analyse(argc,argv);
}



/***********************************************************************
 * 
 **********************************************************************/
void MainAppDriver::initUrgDriver()
{
	
		//! --- Start Hukuyo ---
		ud = UrgDriver::getUrgDriver();
		

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

}

/***********************************************************************
 * 
 **********************************************************************/
void MainAppDriver::waitHere()
{	
	ud->waitHere();    
	cm->waitHere();
}
