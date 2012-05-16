#include "controle_ax12.h"

AX12 motor[NB_MOTEURS];
int reverse[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int goal[NB_MOTEURS];
int ordre[NB_MOTEURS];

int tourner(int id, int id_moteur, int position) {
  if (position < -150 || position > 150 || id_moteur > NB_MOTEURS) {
    return E_INVALID_PARAMETERS_VALUE;
  }
  else {
    int valeur = map(position, -150, 150, 0, 1023);
    goal[id_moteur] = position;
    ordre[id_moteur] = id;
    return motor[id_moteur].writeInfo (GOAL_POSITION, valeur);
  }
}

int get_position(int id, int id_moteur) {
  if (id_moteur > NB_MOTEURS) {
    return E_INVALID_PARAMETERS_VALUE;
  }
  else {
    int valeur = lire (id, PRESENT_POSITION, id_moteur);
    if (valeur == -1)
      return -1;
    else
      return (valeur*0.293255132)-150;
  }
}

int lire(int id, int ordre, int id_moteur) {
  if (id_moteur > NB_MOTEURS) {
    return E_INVALID_PARAMETERS_VALUE;
  }
  else {
    motor[id_moteur].readInfo (ordre);
    return motor[id_moteur].status_data;
  }
}

void cherche_moteurs(void) {
  AX12::init(1000000);

  // on demande la déection de 4 AX12
  int detect[NB_MOTEURS];
  int c = 0;
  AX12::autoDetect (detect, NB_MOTEURS);

  for (byte i=0; i<NB_MOTEURS; i++) {
    sendMessage(8, detect[i]);
    if (detect[i] >= 0) {
      c++;

      // detect[i] est l'id interne du moteur (modifiable si besoin mais à ne pas faire pendant que le robot)
      // i est le numéro du moteur dans la chaine
      motor[i] = AX12(detect[i]);
      reverse[detect[i]] = i;
      goal[i] = 200;
      ordre[i] = -1;

      motor[i].writeInfo (TORQUE_ENABLE, 1); // on doit activer ça sinon le moteur ne développe pas de couple
      motor[i].setEndlessTurnMode(false); // ça semble pas utile: TODO tester sans pour pour voir

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
  sendMessage(0, "detected", c); //on renvoie le nombre de moteurs détectés
}
