/**
 * \file 	argManager.h
 * \author 	Xavier RODRIGUEZ
 * \date	08/03/2012
 * 
 * */
#ifndef ARGMANAGER_H
#define ARGMANAGER_H

#include <list>
#include <string>
#include <iostream>
#include <typeinfo>

using std::cout;
using std::cerr; 
using std::endl;
using std::list;
using std::string;

 
/**
 * \class absArgument
 * 
 * classe dont le seul but est d'abstraire le fait que les arguement sont templates
 * Si vous avez plus propre je suis preneur
 * 
 * */
class absArgument
{
protected:
	string tag;
	bool valid;
	
public:
	absArgument(string t="-arg") :
		tag(t),valid(false) {};
	~absArgument(){};
		
	void validArg(){valid=true;};
	string getTag(){return tag;};	
	virtual bool isValid(){return valid;};		// C'est juste pour le polymorphisme
};
 
/**
 * \class Argument
 * 
 * */
template<typename TYPE>
class Argument : public absArgument
{
	TYPE value;
	
public:
	Argument(string t="-arg") : 
		absArgument(t) {};
	~Argument(){};
	
	TYPE getValue(){return value;};
	void setValue(TYPE v){value = v;};
};


/**
 * \class ArgManager
 * 
 * Pour l'instant je ne gére que 2 types d'arguments, les string et les int
 * 
 * */
class ArgManager
{
	int nbOfArg;
	list<absArgument* > arguments;
	typename list<absArgument* >::iterator it;
	
public:
	ArgManager();
	
	void addArg(absArgument* arg);
	absArgument* getArg(string tag);
	
	void incArgNumber();
	int getNumberOfArg(){return nbOfArg;};
	
	void analyse(int argc, char *argv[]);
	void stopProcess();
	
	
	static const int GET_INT =0;
	static const string GET_STRING;
	
	
	/**
	 * \fn getArg
	 * 
	 * Retourne la valeur de l'argument
	 * Pour l'instant il faut utiliser différentes fonctions suivants le type
	 * du paramétres // je suis sur le coup pour changer ça ^^
	 * */
	Argument<string>* getArg(string tag, string s) {
		for ( it=arguments.begin() ; it!=arguments.end() ; it++ )
		{
			if(!(*it)->getTag().compare(tag))
			{
				Argument<string>* strArg = dynamic_cast<Argument<string>* >((*it));
				if(strArg) {	
					return strArg;
				}	
			}
		}
		cerr << "Erreur argument non trouvé" << endl;
		return NULL;
	}; 
	
	
	Argument<int>* getArg(string tag, int i) {
		for ( it=arguments.begin() ; it!=arguments.end() ; it++ )
		{
			if(!(*it)->getTag().compare(tag))
			{
				Argument<int>* intArg = dynamic_cast<Argument<int>* >((*it));
				if(intArg) {	
					return intArg;
				}	
			}
		}
		cerr << "Erreur argument non trouvé" << endl;
		return NULL;
	};
		
		
	
}; // End class ArgManager
 
 

	 
 
#endif // ARGMANAGER_H
