
#include "WProgram.h"
#include "wiring.h"


#include "message.h"
#include "command.h"


void initSerialLink()
{
	Serial.begin(SERIAL_BAUD);
}


///
/// Envoie un int
///
void sendMessage(int id, int i)
{
	Serial.print(id);
	Serial.print(SEP);
	Serial.println(i);
}


///
/// Envoie un string
///
void sendMessage(int id, char* str)
{
	Serial.print(id);
	Serial.print(SEP);
	Serial.println(str);
}

///
/// Envoie un int et un string
///
void sendMessage(int id, int i, char* str)
{
	Serial.print(id);
	Serial.print(SEP);
	Serial.print(i);
	Serial.print(SEP);
	Serial.println(str);
}

///
/// Envoie un string et un int
///
void sendMessage(int id, char* str, int i)
{
	Serial.print(id);
	Serial.print(SEP);
	Serial.print(str);
	Serial.print(SEP);
	Serial.println(i);
}


///
/// Envoie un tableau d'int
///
void sendMessage(int id, int *tabi, int size)
{
	Serial.print(id);
	Serial.print(SEP);
	for (int i=0; i<size-1; ++i)
	{
		Serial.print(tabi[i]);
		Serial.print(SEP);
	}
	Serial.println(tabi[size-1]);
}

///
/// Envoie des strings et des int
/// aucune protection, il faut au moins envoyer une chaine et un int
///
void sendMessage(int id, char** tabs, int nbStr, int *tabi, int nbInt)
{
	Serial.print(id);
	Serial.print(SEP);
	
	for (int i=0; i<nbStr; ++i)
	{
		Serial.print(tabs[i]);
		Serial.print(SEP);
	}
	
	for (int i=0; i<nbInt-1; ++i)
	{
		Serial.print(tabi[i]);
		Serial.print(SEP);
	}
	Serial.println(tabi[nbInt-1]);
}

///
/// Envoie des int et des strings
/// aucune protection, il faut au moins envoyer une chaine et un int
///
void sendMessage(int id, char cmd, int* tabi, int nbInt, char** tabs, int nbStr)
{
	Serial.print(id);
	Serial.print(SEP);
	
	for (int i=0; i<nbInt; ++i)
	{
		Serial.print(tabi[i]);
		Serial.print(SEP);
	}
	
	for (int i=0; i<nbStr-1; ++i)
	{
		Serial.print(tabs[i]);
		Serial.print(SEP);
	}
	Serial.println(tabs[nbStr-1]);
}

///
/// Parse les donnees recues sur le port serie et appel la fonction cmd pour effectuer les traitements
///
void readIncomingData()
{
	static char currentArg[10]; //<= cedric : je reduis ici pour menager la memoire des arduino (vu qu'on les cast en entier avec atoi, le max c'est 6 normalement (-32000 -> +32000)
	static int args[10];
	static int argsIndex = 0;
	static int currentArgIndex = 0;

	/*
	 * A propos du protocole :
	 * id_cmd:arg1:arg2:...
	 * - un message se termine par \n
	 */

	// s'il y a des donnees a lire
	int available = Serial.available();
	for(int i = 0; i < available; i++){
		// recuperer l'octet courant
		int data = Serial.read();
		switch(data){
			// separateur
			case SEP:
			{
			   	currentArg[currentArgIndex] = '\0';
	   			args[argsIndex] = atoi(currentArg);
				argsIndex++;
				currentArgIndex = 0;
				break;
			}
			// fin de trame
			case '\n':
			{
				currentArg[currentArgIndex] = '\0';
				args[argsIndex] = atoi(currentArg);
				cmd(args[0],args[1],args+2,argsIndex-1); // id_cmd, *args, sizeArgs
  				argsIndex = 0;
				currentArgIndex = 0;
				break;
			}
			default:
			{
				currentArg[currentArgIndex] = data;	
				currentArgIndex++;
				break;
			}
		}
	}
}


