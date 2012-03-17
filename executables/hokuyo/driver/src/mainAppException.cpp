/**
 * \file 	mainAppException.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	13/03/2012
 * 
 * <p></p>
 * 
 * */
 
#include "mainAppException.h"

#define MAX_ERROR 10

int mainAppException::nbExThrow = 0;


mainAppException::mainAppException(MainAppDriver* mad,int type)
{
	this->a = mad;
	this->typeErr = type;
	mainAppException::nbExThrow++;
}



void mainAppException::react(void)
{
	
	if(mainAppException::nbExThrow > MAX_ERROR) {
		//
		
		finalKill();
	}
	// Traitement des erreurs en fonctions de leurs sources
	switch(this->typeErr)
	{
		//
		case mainAppException::Err_urgException_react_tropDerr :{
		
			cerr << "Comportement à définir" << endl;
				finalKill();
		break;}
		
	}	

	
}



void mainAppException::finalKill()
{
	cerr << "KILL VIOLENT - il n'y a plus rien à faire pour sauver le process" << endl;
	a->cm->autoKill();
}
