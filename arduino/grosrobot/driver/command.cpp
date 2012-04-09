#include "command.h"


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
		case QO_BOUGER_PEIGNE:
		{
			int res = 0;
			long int goal = 0;
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				goal = (long int)args[0]*(long int)1023/(long int)300;
				res = bougerPeigne(goal, args[1], AX18_PEIGNE_G);
				if(res > 0)
				{
					sendMessage(id, E_MERDE_AX18);
				} 
				res = bougerPeigne(goal, args[1], AX18_PEIGNE_D); 
				if(res > 0)
				{
					sendMessage(id, E_MERDE_AX18);
				}
			}
			break;
		}
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
