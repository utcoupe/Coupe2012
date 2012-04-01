/**
 * \file 	comManager.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	08/03/2012
 * 
 * */
 
#include "comManager.h"

#include <string>
#include <stdlib.h>

using namespace std;

//! Initialisation du singleton
ComManager* ComManager::objCom = 0;

//! ---
ComManager* ComManager::getComManager()
{
	if(objCom == 0) {
		objCom = new ComManager();
		return objCom;
	}
	else{
		return objCom;
	}		
}

//! --- 
ComManager::ComManager()
{
	command=0;
	pthread_mutex_init(&(this->mutex), NULL);
}

//! ---
ComManager::~ComManager()
{
	
}

//! ---
void ComManager::addKill(int id)
{
	this->killCmd = id;
}	

//! --- 
void ComManager::addFunction(int id,void(*fct)())
{
	this->fonctions.push_front(pair<int,void(*)()>(id,fct));
}

//! --- 
void* ComManager::helpfct(void* arg)
{
	return getComManager()->loop(arg);
}

//! --- 
void ComManager::start()
{
	if(pthread_create(&thr, NULL, &ComManager::helpfct, NULL)) {
		cerr << "Erreur : Impossible de créer le Thread Com" << endl;
    }
}

void ComManager::waitHere()
{
	if(pthread_join(thr, NULL)) {
		cerr << "Erreur : Impossible de joindre le Thread Com" << endl;;
    }
}

//! ---
void* ComManager::loop(void* arg)
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
