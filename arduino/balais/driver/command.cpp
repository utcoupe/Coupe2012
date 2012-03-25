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
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				ouvrirBalais(args[0], 0); // 0 et 1 = numero du moteurs A MODIF ! argument recu = vitesse
				ouvrirBalais(args[0], 1); // on ouvre chacun des deux balais
			}
			break;
		}
		case Q_FERMERBALAIS:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				fermerBalais(args[0], 0); // 0 et 1 = numero moteurs : A MODIF ! argument recu = vitesse
				fermerBalais(args[0], 1); // on ferme chacun des deux balais
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
