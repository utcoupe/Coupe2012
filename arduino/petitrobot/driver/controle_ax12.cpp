#include "controle_ax12.h"

AX12 motor[NB_MOTEURS] = {AX12 (), AX12 (), AX12(), AX12()};

byte tourner(int id, int id_moteur, int position) {
  if (position < -150 || position > 150 || id_moteur > NB_MOTEURS) {
    sendMessage(id, E_INVALID_PARAMETERS_VALUE);
    return 0;
  }
  else {
    return motor[id_moteur].writeInfo (GOAL_POSITION, map(position, -150, 150, 0, 1023));
  }
}


void cherche_moteurs(void) {
  AX12::init(1000000);

  // on détecte automatiquement 2 AX12
  int detect[NB_MOTEURS];
  AX12::autoDetect (detect, NB_MOTEURS);
  for (byte i=0; i<NB_MOTEURS; i++) {
    sendMessage(0, i);
    motor[i].id = detect[i];
    motor[i].writeInfo (TORQUE_ENABLE, 1);               // habilita el torque
    motor[i].setEndlessTurnMode(false);                   // lo pone en modo de rotación continua
    // setear la inercia y todo eso
    motor[i].writeInfo (CW_COMPLIANCE_MARGIN, 0);
    motor[i].writeInfo (CCW_COMPLIANCE_MARGIN, 0);
    motor[i].writeInfo (CW_COMPLIANCE_SLOPE, 95);
    motor[i].writeInfo (CCW_COMPLIANCE_SLOPE, 95);
    motor[i].writeInfo (PUNCH, 150);
    motor[i].writeInfo (MAX_TORQUE, 1023);
    motor[i].writeInfo (LIMIT_TEMPERATURE, 85);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 60);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 190);
    motor[i].writeInfo (RETURN_DELAY_TIME, 150);
  }
}
