/**
 * \file 	urgException.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	13/03/2012
 * 
 * <p></p>
 * 
 * */
 
#include "urgException.h"
#include "mainAppException.h"


extern MainAppDriver* application;

#define MAX_ERROR 100

int urgException::nbExThrow = 0;



urgException::urgException(UrgDriver* u,int type)
{
	this->ud = u;
	this->typeErr = type;
	urgException::nbExThrow++;
}


/***********************************************************************
 * 
 **********************************************************************/
void urgException::react(void)
{
	try{	
		if(urgException::nbExThrow > MAX_ERROR) {
			throw new mainAppException(application,mainAppException::Err_urgException_react_tropDerr);
		}
		// Traitement des erreurs en fonctions de leurs sources
		switch(this->typeErr)
		{
			//
			case urgException::Err_start_threadPb :{
				cerr << "Erreur : Impossible de créer le Thread Urg" << endl;
				// TODO
				// Fonction de gestion de cette erreur
			break;}
			//
			case urgException::Err_loop_urgNoConnect :{
				cerr << "Erreur : UrgDriver::loop : urg non connecté" << endl;
				this->connectUrgDevice();
			break;}
			//
			case urgException::Err_connectHokuyo_urgNoConnect :{
				cerr << "Erreur : UrgDriver::connectHokuyo : urg non connecté" << endl;
				this->findAndConnectOneUrgDevice();
			break;}
		}	
	}	
	catch(mainAppException* e){
		e->react();
	}
}

/***********************************************************************
 * <p>Méthode qui relance la tentative de connection au cas où</p>
 **********************************************************************/
void urgException::connectUrgDevice()
{
	cerr << "Tentative de correction" << endl;
	cerr << "Nouvelle tentative de connection avec les même paramétres" << endl;
	this->ud->connectHokuyo();
	cerr << "Connection OK, lancement de l'analyse" << endl;
	this->ud->start();
}

/***********************************************************************
 * <p>Méthode qui cherche à connécter l'hokuyo à tout prix</p>
 **********************************************************************/
void urgException::findAndConnectOneUrgDevice()
{
	cerr << "Tentative de correction" << endl;
	cerr << "Recherche d'un nouveau port com" << endl;
	this->ud->setComPort(ud->hokuyoFindPortCom(10));
	cerr << "Tentative de connection avec le nouveau port com" << endl;
	this->ud->connectHokuyo();
}

