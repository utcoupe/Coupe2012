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
	UrgDriver * ud;

public:

	static int nbExThrow;
	
	
	urgException(UrgDriver* u,int type);
	
	
};


#endif // URGEXCEPTION_H
