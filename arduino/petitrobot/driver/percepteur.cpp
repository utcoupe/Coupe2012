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
  this->motors = (AF_DCMotor*)malloc(sizeof(AF_DCMotor) * NB_MOTORS_BALAIS);
  for (int i=0; i<NB_MOTORS_BALAIS; ++i) {
    this->motors[i] = AF_DCMotor(i+1, MOTOR12_64KHZ);
    this->motors[i].setSpeed(0);
    this->motors[i].run(FORWARD);
  }
}

// y a pas juste une fonction pour démarrer ?
int Percepteur::set_pwm(int _ordre, int _pwm) {
  id_ordre = _ordre;

  (if _pwm == 0) state = 0;

  this->pwm = abs(min(255,_pwm));
  if (_pwm < 0) {
    this->sens = BACKWARD;
    if (etat_switchs != 0)
      etat_moteur = 1;
    else
      return 0;
  }
  else {
    this->sens = FORWARD;
    if (etat_switchs != 2)
      etat_moteur = 2;
    else
      return 0;
  }

  //s'occuper des switchs fin de course avant de (éventuellement) lancer la pwm
  //stocker l'id de l'ordre quelquepart pour envoyer un message quand on arrive en fdc

  for (int i=0; i<NB_MOTORS_BALAIS; ++i) {
    this->motors[i].setSpeed(this->pwm);
    this->motors[i].run(this->sens);
  }

  return 1;
}

void fdcHaut() {
  if (digitalRead(18)) { //switch à 1
    etat_switchs = 2;
    sendMessage(id_ordre, "haut atteint");
    if (etat_moteur == 1)
      g_percepteur.set_pwm(0);
  } else {
    sendMessage(id_ordre, "Pu en haut");
  }
}

void fdcBas() {
  if (digitalRead(19)) { // switch a 1
    etat_switchs = 0;
    sendMessage(id_ordre, "bas atteint");
    if (etat_moteur == 2)
      g_percepteur.set_pwm(0);
  } else {
    sendMessage(id_ordre, "Pu en bas");
  }
}
