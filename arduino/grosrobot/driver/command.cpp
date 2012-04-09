#include "command.h"

#include "balais.h"
#include "tapis.h"

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
		case QO_ID:
		{
			sendMessage(id, "others");
			break;
		}
		case QO_PING:
		{
			sendMessage(id, "pong");
			break;
		}
		case QO_BALAIS:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				sendMessage(id,g_balais.set_pwm(args[0]));
			}
			break;
		}
		case QO_TAPIS:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				sendMessage(id,g_tapis.actionner(args[0]));
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
