/**
 * \file 	checkParameters.h
 * \author 	Xavier RODRIGUEZ
 * \date	19/02/2012
 * 
 * Fonctions de vérification des paramétres donné à la fonction
 * 
 * */
#ifndef CHECKPARAMETERS_H
#define CHECKPARAMETERS_H

// Structure contenant les paramétres modifiables
struct MainParameters
{
	string 	comPort;
	int 	color;	
};

// Parametres passables à l'application
MainParameters appParameter;

inline
int checkParameters(MainParameters* mp, int argc, char *argv[])
{
	// Initialisation des parametres
	mp->comPort = string("");
	mp->color = -1;
	
	// Lecture des paramétres
	int nbParam = (argc-1)/2;
	int acParam = 0;
	while(acParam<nbParam && nbParam>0)
	{
		acParam++;
		char* argument = argv[acParam];
					
		// Port com
		if(!strcmp(argument,"-com"))
		{
			mp->comPort = std::string(argv[acParam+1]);
		}
		
		// Couleur
		if(!strcmp(argument,"-col"))
		{
			string strCol = std::string(argv[acParam+1]);
			int intCol = atoi(strCol.c_str());
			if( intCol==ROUGE || intCol==VIOLET ){
				mp->color = intCol;
			}
		}
			
    }
	
	#if DEBUG
	cout << endl;
	cout << "--- Résumé des paramétres ---" << endl;
	cout << "Port Com : " << mp->comPort << endl;
	cout << "Couleur : " << mp->color << endl;
	cout << endl;
	#endif

	return 0;
}



#endif
