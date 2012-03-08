/**
 * \file 	comManager.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	08/03/2012
 * 
 * */

#include <list>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

#include "comManager.h"

//! Initialisation du singleton
comManager* comManager::objCom = 0;

//! ---
comManager* comManager::getComManager()
{
	if(objCom == 0) {
		objCom = new comManager();
		return objCom;
	}
	else{
		return objCom;
	}		
}

//! --- 
comManager::comManager()
{
	command=0;
	pthread_mutex_init(&(this->mutex), NULL);
}

//! ---
void comManager::addKill(int id)
{
	this->killCmd = id;
}	

//! --- 
void comManager::addFunction(int id,void(*fct)())
{
	this->fonctions.push_front(pair<int,void(*)()>(id,fct));
}

//! --- 
void* comManager::helpfct(void* arg)
{
	return getComManager()->loop(arg);
}

//! --- 
void comManager::start()
{
	if(pthread_create(&thr, NULL, &comManager::helpfct, NULL)) {
		cerr << "Error : Unable to create thread Com" << endl;
    }
    
    if(pthread_join(thr, NULL)) {
		cerr << "Error : Could not join thread Com" << endl;;
    }
}

//! ---
void* comManager::loop(void* arg)
{	
	size_t pos;	
	for( ; ; )
	{
		// Command Recovery
		cin >> request;
		pos = request.find(SEP); 
		this->idCmd   = atoi(request.substr(0,pos).c_str());
		this->command = atoi(request.substr(pos+1).c_str()); 
			
		cout << idCmd << SEP << ("(");	
		for ( ite=fonctions.begin() ; ite!=fonctions.end() ; ite++ )
		{
			if( command == (*ite).first )
			{
				pthread_mutex_lock(&(this->mutex));
				(*((*ite).second))();
				pthread_mutex_unlock(&(this->mutex));
			}
		}
		cout << ")" << endl; 
		cout.flush();
		
		// KILL
		if(command==killCmd)
		{
			return NULL;
		}
	}
	return NULL;
}
