/**
 * \file 	comManager.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * */
#ifndef COMMANAGER_H
#define COMMANAGER_H

#define SEP "."


/**
 * \class comManager
 * 
 * classe de gestion de la communication en C++
 * */
class comManager
{

private:
	int idCmd;
	int killCmd;
	int command;
	string request;
	ostringstream mess;
	
	list<pair<int,void(*)()> > fonctions;
	list<pair<int,void(*)()> >::iterator ite;
	
	
public:
	// Création de l'objet avec commande de kill obligatoire
	comManager(int killCmd);				
	void addFunction(int id,void(*)());
	void* loop(void* arg);
	
};

//!
comManager::comManager(int kC)
{
	killCmd = kC;
	command=0;
}

//!
void comManager::addFunction(int id,void(*fct)())
{
	this->fonctions.push_front(pair<int,void(*)()>(id,fct));
}

//!
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
			
		mess.str(""); 
		cout << idCmd << SEP << ("(");
		
		//cout << mess.str();
				
		for ( ite=fonctions.begin() ; ite!=fonctions.end() ; ite++ )
		{
			if( command == (*ite).first )
			{
				(*((*ite).second))();
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


#endif // COMMANAGER_H
