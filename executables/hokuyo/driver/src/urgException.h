/**
 * \file 	urgException.h
 * \author 	Xavier RODRIGUEZ
 * \date	13/03/2012
 * 
 * <p></p>
 * 
 * */
#ifndef URGEXCEPTION_H
#define URGEXCEPTION_H

#include "urgDriver.h"

#include <iostream>
#include <exception>

using namespace std;

class urgException: public exception
{
	
public:

	static int nbExThrow;
	
	// Types d'erreur
	static const int Err_start_threadPb 							= 10;
	static const int Err_loop_urgNoConnect 							= 11;
	static const int Err_connectHokuyo_urgNoConnect 				= 12;
	
	// 
	urgException(UrgDriver* u,int type);
	void react(void);
	
private:
	UrgDriver * ud;
	int typeErr;
	
	// Fonctions de correction des problémes
	void connectUrgDevice();
	void findAndConnectOneUrgDevice();
	
};


#endif // URGEXCEPTION_H
