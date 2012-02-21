#ifndef CHECKPARAMETERS_H
#define CHECKPARAMETERS_H
/**
 * Fonctions de verification et de récupération des paramétres
*/

struct MainParameters
{
	std::string comPort;
	
};

int checkParameters(MainParameters* mp, int argc, char *argv[])
{
	
	// Initialisation des parametres
	mp->comPort = std::string("/dev/ttyACM0");
	
	// Read Parameters
	int nbParam=(argc-1)/2;
	int param=0;
	while(param<nbParam && nbParam>0)
	{
		//
		param++;
		char* argument = argv[param];
					
		// Port com
		if(!strcmp(argument,"-com"))
		{
			mp->comPort = std::string(argv[param+1]);
		}
		
    }
	

#if DEBUG

	std::cout << "Port Com : " << mp->comPort << std::endl;

#endif

  return 0;
}



#endif
