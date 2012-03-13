/**
 * \file 	urgException.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	13/03/2012
 * 
 * <p></p>
 * 
 * */
 

#include "urgException.h"


int urgException::nbExThrow = 0;



urgException::urgException(UrgDriver* u,int type)
{
	this->ud = u;
	urgException::nbExThrow++;
	
	
	cout << type << endl;
	cout << nbExThrow << endl;
}
