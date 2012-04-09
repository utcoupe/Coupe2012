/* Commande balais par AX-18
 * Par Quentin C
 * Le 09/04/2012 */
 
 //Controle basé sur la controle des AX12 : a tester avec AX18

#include "controleAX18.h"

AX12 motor[NB_MOTEURS] = {AX12 (), AX12 (), AX12 ()};

void cherche_moteurs(void) {
  AX12::init(1000000);

  // on détecte automatiquement 2 AX12 (ici des AX18)
  int detect[NB_MOTEURS];
  AX12::autoDetect (detect, NB_MOTEURS);
  for (byte i=0; i<NB_MOTEURS; i++) {
    motor[i].id = detect[i];
    motor[i].writeInfo (TORQUE_ENABLE, 1);               // habilita el torque
    motor[i].setEndlessTurnMode(false);                   // lo pone en modo de rotación continua
    // setear la inercia y todo eso
    motor[i].writeInfo (CW_COMPLIANCE_MARGIN, 0);
    motor[i].writeInfo (CCW_COMPLIANCE_MARGIN, 0);
    motor[i].writeInfo (CW_COMPLIANCE_SLOPE, 95);
    motor[i].writeInfo (CCW_COMPLIANCE_SLOPE, 95);
    motor[i].writeInfo (PUNCH, 150);
    motor[i].writeInfo (MAX_TORQUE, 0);
    motor[i].writeInfo (LIMIT_TEMPERATURE, 85);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 60);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 190);
    motor[i].writeInfo (RETURN_DELAY_TIME, 150);
  }
}


int bougerPeigne(int angle, int vitesse, int moteur)  
{
	int res = 0;
		
		res = motor[moteur].writeInfo (MOVING_SPEED, vitesse);
		res = motor[moteur].writeInfo (GOAL_POSITION, angle); 
		return res;
}
