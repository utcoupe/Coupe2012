/**
 * \file 	sample.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	01/04/2012          // et c'est pas une blague !!!	
 * 
 * */

#include "comManager.h"


#define CMD_KILL  666


/*
 * Les fonctions de commandes sont de type  
 * 			void nomFonction(void) 
 * 
 * Pour communiquer il suffit de faire les cout dans le corps de la 
 * fonction!
 * 
 * !!! ne jamais faire de cout << endl !!!
 * Pour comprendre voir fonction 2
 * 
 * */
 
 
#define CMD1 1
void fonction1() {
	
	
	cout << "Infos de la fonction 1";
	
}

#define CMD2 2
void fonction2() {
	
	
	cout << "Infos de la fonction 2" << endl;
	
}


int main() {
	
	// Pas de new, il faut récupérer le singleton
	// c'est l'unique instance du manager
	ComManager* cm = ComManager::getComManager();
	
	// Premiére commande à mettre le kill
	// c'est la commande qui stop le manager
	cm->addKill(CMD_KILL);
	
	
	cm->addFunction(CMD1,&fonction1);
	cm->addFunction(CMD2,&fonction2);
	
	
	/*
	Pour partager 1 mutex entre 2 processus
	il est possible de récupérer le mutex du manager   			getMutex
	il est aussi possible de lui appliquer un mutex déja créé 	setMutex
	
	cm->setMutex(mutex_Dun_Autre_Processus);					
	
	*/
	
	// Une fois tous les paramétres ok, on lance le manager
	cm->start();
	
	
	
	/*
	 * 
	 * Du code... du code... plein de code
	 * 
	 * */
	
	
	
	// Ne pas oublier le wait final 
	cm->waitHere();
	
	return 0;
}
