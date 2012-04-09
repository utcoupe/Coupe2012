#include "percepteur.h"

Percepteur g_percepteur;
int id_ordre = -1;

/* etats switchs
0 : bas
1 : bas hésitation
2 : haut
3 : haut hésitation
4 : au milieu
*/
int etat_switchs = 4;

/* etats moteur
0 : arrêté
1 : elevator going up!
2 : backward
 */
int etat_moteur = 0;

Percepteur::Percepteur() {
  this->pwm = 0;
  this->sens = FORWARD;
  this->motors = (AF_DCMotor*)malloc(sizeof(AF_DCMotor) * NB_MOTORS_PERCEPTEUR);
  for (int i=0; i<NB_MOTORS_PERCEPTEUR; ++i) {
    this->motors[i] = AF_DCMotor(i+1, MOTOR12_64KHZ);
    this->motors[i].setSpeed(0);
    this->motors[i].run(FORWARD);
  }
}

// y a pas juste une fonction pour démarrer ?
int Percepteur::set_pwm(int _ordre, int _pwm) {
  id_ordre = _ordre;

  if (_pwm == 0) etat_moteur = 0;

  this->pwm = abs(min(255,_pwm));
  if (_pwm < 0) {
    this->sens = BACKWARD;
    if (etat_switchs != 0) {
      etat_moteur = 2;
      sendMessage(_ordre, "go backward");
    }
    else
      return 0;
  }
  else if (_pwm > 0) {
    this->sens = FORWARD;
    if (etat_switchs != 2) {
      etat_moteur = 1;
      sendMessage(_ordre, "go forward");
    }
    else
      return 0;
  }

  for (int i=0; i<NB_MOTORS_PERCEPTEUR; ++i) {
    this->motors[i].setSpeed(this->pwm);
    this->motors[i].run(this->sens);
  }

  return 1;
}

void fdcHaut() {
  if (!digitalRead(PIN_SWITCH_HAUT)) { //switch on
    if (etat_switchs != 2) {
      etat_switchs = 2;
      sendMessage(id_ordre, "haut atteint", etat_moteur);

      if (etat_moteur == 1)
	g_percepteur.set_pwm(id_ordre, 0);
    }
  } else {
    sendMessage(id_ordre, "Pu en haut");
    etat_switchs = 4;
  }
}

void fdcBas() {
  if (!digitalRead(PIN_SWITCH_BAS)) { // switch on
    if (etat_switchs != 0) {
      etat_switchs = 0;
      sendMessage(id_ordre, "bas atteint", etat_moteur);
      if (etat_moteur == 2)
	g_percepteur.set_pwm(id_ordre, 0);
    }
  } else {
    sendMessage(id_ordre, "Pu en bas");
    etat_switchs = 4;
  }
}
