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
	case QO_IDENT:
	{
	    sendMessage(id, ID_OTHERS, (char*)"others");
	    break;
	}
	case QO_PING:
	{
	    sendMessage(id, (char*)"Pong");
	    break;
	}
	case QO_RESET:
	{
	    sendMessage(id, 1);
	    break;
	}
	default:
	{
	    sendMessage(id,-1);
	    break;
	}
    }
}
