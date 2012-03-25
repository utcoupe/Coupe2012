/* Commande balais par AX-18
 * Par Quentin C
 * Le 19/03/2012 */
 
 //Les moteurs ne sont pas initialisés ici
 //Controle basé sur la controle des AX12 : a tester avec AX18

#include "Arduino.h"
#include "balais.h"

void ouvrirBalais(int vitesse, int moteur);  
{
	int res = 0;
	
		res = motor[moteur].writeInfo (MOVING_SPEED, vitesse); // vitesse d'ouverture 0-1023
		res = motor[moteur].writeInfo (GOAL_POSITION, 1023); // remplacer 1023 par la position ouverte
}

void fermerBalais(int vitesse, int moteur); 
{
	int res = 0;
	
	res = motor[moteur].writeInfo (MOVING_SPEED, vitesse); // vitesse d'ouverture 0-1023
	res = motor[moteur].writeInfo (GOAL_POSITION, 0); // remplacer 0 par la position ouverte
}
