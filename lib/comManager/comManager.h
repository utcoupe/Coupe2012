/**
 * \file 	comManager.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 *
 * */

#ifndef COMMANAGER_H
#define COMMANAGER_H
#include <iostream>
#include <pthread.h>
#include <list>

using namespace std;

/**
 * \class ComManager
 *
 * classe de gestion de la communication en C++
 * */
class ComManager
{

private:
	static ComManager* objCom;

	int idCmd;
	int killCmd;
	int command;
	string request;

	pthread_t thr;
	pthread_mutex_t mutex;

	list<pair<int,void(*)()> > fonctions;
	list<pair<int,void(*)()> >::iterator ite;

	ComManager();
	~ComManager();

	void* loop(void* arg);

public:

	static ComManager* getComManager();

	void addKill(int id);
	void addFunction(int id,void(*)());

	// On peut utiliser directement la fonction dans un thread perso
	static void* helpfct(void* arg);
	// Ou lancer le automatiquement le thread du manager
	void start();
	void waitHere();

	pthread_mutex_t getMutex(){return this->mutex;};
	void setMutex(pthread_mutex_t m){this->mutex=m;};

	void autoKill(){
		throw 666;
	};
};




#endif // COMMANAGER_H
