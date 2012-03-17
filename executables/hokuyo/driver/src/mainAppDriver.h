


#include "urgDriver.h"
#include "argManager.h"
#include "comManager.h"

/***********************************************************************
 * \class MainAppDriver
 * 
 * <h1>MainAppDriver</h1>
 * <p>Classe qui gére l'ensemble du driver</p>
 **********************************************************************/
class MainAppDriver
{

public:

	// Déclaration des Managers
	ArgManager* am;
	ComManager* cm;
	UrgDriver * ud;
	
	MainAppDriver(int argc, char *argv[]);

	void initArgManager(int argc, char *argv[]);	
	void initComManager();
	void initUrgDriver();
	
	void waitHere();
};



