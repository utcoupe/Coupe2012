/**
 * \file 	mainAppException.h
 * \author 	Xavier RODRIGUEZ
 * \date	13/03/2012
 * 
 * <p></p>
 * 
 * */
#ifndef MAINAPPEXCEPTION_H
#define MAINAPPEXCEPTION_H

#include <iostream>
#include <exception>

#include "mainAppDriver.h"

using namespace std;

class mainAppException: public exception
{
	
public:

	static int nbExThrow;
	
	// Types d'erreur
	static const int Err_urgException_react_tropDerr 				= 50;

	// 
	mainAppException(MainAppDriver* u,int type);
	void react(void);
	
private:
	MainAppDriver * a;
	int typeErr;
	
	// Fonctions de correction des problémes


	void finalKill();
	
};




#endif // MAINAPPEXCEPTION_H
