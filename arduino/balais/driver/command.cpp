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
		case Q_OUVRIRBALAIS:
		{
			int res = 0;
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				res = res + ouvrirBalais(args[0], args[1], 0); // 0 et 1 = numero du moteurs A MODIF ! argument recu = vitesse
				res = res + ouvrirBalais(args[0], args[1], 1); // on ouvre chacun des deux balais
				if(res >0)
				{
					sendMessage(id, E_MERDE_AX18);
				}
			}
			break;
		}
		case Q_FERMERBALAIS:
		{
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				res = res + fermerBalais(args[0], args[1], 0); // 0 et 1 = numero du moteurs A MODIF ! argument recu = vitesse
				res = res + fermerBalais(args[0], args[1], 1); // on ouvre chacun des deux balais
				if(res >0)
				{
					sendMessage(id, E_MERDE_AX18);
				}
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
