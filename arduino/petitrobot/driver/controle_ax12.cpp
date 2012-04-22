#include "controle_ax12.h"

AX12 motor[NB_MOTEURS] = {AX12 (), AX12 (), AX12(), AX12()};
int reverse[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

byte tourner(int id, int id_moteur, int position) {
  if (position < -150 || position > 150 || id_moteur > NB_MOTEURS) {
    return E_INVALID_PARAMETERS_VALUE;
  }
  else {
    return motor[id_moteur].writeInfo (GOAL_POSITION, map(position, -150, 150, 0, 1023));
  }
}

byte get_position(int id, int id_moteur) {
  if (id_moteur > NB_MOTEURS) {
    return E_INVALID_PARAMETERS_VALUE;
  }
  else {
    return motor[id_moteur].readInfo (PRESENT_POSITION);
  }
}

void cherche_moteurs(void) {
  AX12::init(1000000);

  // on demande la déection de 4 AX12
  int detect[NB_MOTEURS];
  int c = 0;
  AX12::autoDetect (detect, NB_MOTEURS);

  for (byte i=0; i<NB_MOTEURS; i++) {
    if (detect[i] >= 0) {
      c++;

      // detect[i] est l'id interne du moteur (modifiable si besoin mais à ne pas faire pendant que le robot)
      // i est le numéro du moteur dans la chaine
      motor[i].id = detect[i];
      reverse[detect[i]] = i;

      motor[i].writeInfo (TORQUE_ENABLE, 1); // on doit activer ça sinon le moteur ne développe pas de couple
      motor[i].setEndlessTurnMode(false); // ça semble pas utile: TODO tester sans pour pourvoir

      // CW : ClockWise
      // et donc CCW : Counter-CLockWise

      // on veut que le moteur aille EXACTEMENT au bon endroit
      motor[i].writeInfo (CW_COMPLIANCE_MARGIN, 0);
      motor[i].writeInfo (CCW_COMPLIANCE_MARGIN, 0);

      // euh ça sert pour l'asservissement apparemment
      // ratio couple/erreur de position
      motor[i].writeInfo (CW_COMPLIANCE_SLOPE, 95);
      motor[i].writeInfo (CCW_COMPLIANCE_SLOPE, 95);

      motor[i].writeInfo (PUNCH, 150);

      // vitesse maximale (dépend du courant fourni évidemment)
      motor[i].writeInfo (MAX_TORQUE, 1023);

      motor[i].writeInfo (LIMIT_TEMPERATURE, 85); //valeur par défaut
      motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 60); //valeur par défaut
      motor[i].writeInfo (UP_LIMIT_VOLTAGE, 190); //valeur par défaut

      motor[i].writeInfo (RETURN_DELAY_TIME, 150);
    }
  }
  sendMessage(0, "detected", c);
}
