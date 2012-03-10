#include "command.h"
#include "led.h"

/* Analyse le message et effectue les actions associees
 * 	<> id : l'identifiant associe au message
 * 	<> header : le type de message (en-tete)
 * 	<> args : le tableau d'entier contenant les arguments
 * */


// from, id_cmd, *args, sizeArgs
void cmd(int id, int header, int *args, int size){
    /* On analyse le message en fonction de son type */
    switch(header)
    {
		case Q_ALLUME:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				allumeLed(args[0]);
			}
			break;
		}
		case Q_ETEINDRE:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				eteindreLed(args[0]);
			}
			break;
		}
		default:
		{
			sendMessage(id,-1);
			break;
		}
    }
}
