#include "command.h"
#include "led.h"
#include "pwm.h"
#include "include_arduino.h"

/* Analyse le message et effectue les actions associees
 * 	<> id : l'identifiant associe au message
 * 	<> header : le type de message (en-tete)
 * 	<> args : le tableau d'entier contenant les arguments
 * */


int consigneGauche=0, consigneDroite=0;
int haut=0;
int gauche=0;
int droite=0;
int bas=0;

// from, id_cmd, *args, sizeArgs
void cmd(int id, int header, int *args, int size){
    /* On analyse le message en fonction de son type */
    switch(header)
    {
		case Q_ID:
		{
			sendMessage(id, "samplebot");
			break;
		}
		case Q_PING:
		{
			sendMessage(id, "pong");
			break;
		}
		case Q_PWM: //haut gauche droite bas
		{
			haut=args[0];
			gauche=args[1];
			droite=args[2];
			bas=args[3];
			
			if(haut && bas)
			{
				consigneGauche=100;
				consigneDroite=100;
			}
			else if(haut && gauche && droite)
			{
				consigneGauche=100;
				consigneDroite=100;
			}
			else if(haut && droite)
			{
				consigneGauche=100;
				consigneDroite=50;
			}	
			else if(haut && gauche)
			{
				consigneGauche=50;
				consigneDroite=100;
			}
			else if(droite)
			{
				consigneGauche=100;
				consigneDroite=-100;
			}	
			else if(gauche)
			{
				consigneGauche=-100;
				consigneDroite=100;
			}
			else if(haut)
			{
				consigneGauche=100;
				consigneDroite=100;
			}
			else if(bas && gauche && droite)
			{
				consigneGauche=-100;
				consigneDroite=-100;
			}
			else if(bas && gauche)
			{
				consigneGauche=-50;
				consigneDroite=-100;
			}
			else if(bas && droite)
			{
				consigneGauche=-100;
				consigneDroite=-50;
			}	
			else if(bas)
			{
				consigneGauche=-100;
				consigneDroite=-100;
			}	
			else if(!haut && !gauche && !droite && !bas)
			{
				consigneGauche=0;
				consigneDroite=0;
			}
			else
			{
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);//echec
				consigneGauche=0;
				consigneDroite=0;
			}
			break;
		}
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
