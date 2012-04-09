/* Commande balais par AX-18
 * Par Quentin C
 * Le 09/04/2012 */
 
 //Les moteurs ne sont pas initialisés ici
 //Controle basé sur la controle des AX12 : a tester avec AX18

#include "Arduino.h"
#include "OFbalais.h"
#include "ax12.h"

void ouvrirBalais(int degre, int vitesse, int moteur);  
{
	int res = 0;
	int pos_actuelle = 0;
		
		res = motor[moteur].readInfo(PRESENT_POSITION);
		pos_actuelle = motor[moteur].status_data;

		res = motor[moteur].writeInfo (MOVING_SPEED, vitesse);
		res = motor[moteur].writeInfo (GOAL_POSITION, pos_actuelle + degre); 
}

void fermerBalais(int, degre, int vitesse, int moteur); 
{
	int res = 0;
	int pos_actuelle = 0;
	
		res = motor[moteur].readInfo(PRESENT_POSITION);
		pos_actuelle = motor[moteur].status_data;
	
		res = motor[moteur].writeInfo (MOVING_SPEED, vitesse); 
		res = motor[moteur].writeInfo (GOAL_POSITION, pos_actuelle - degre);
}
