#ifndef _CONTROLE_AX18_H_
#define _CONTROLE_AX18_H_

#include "ax12.h"
#include "wiring.h"
#include "WProgram.h"

#define NB_MOTEURS 3

extern AX12 motor[NB_MOTEURS];

void cherche_moteurs(void);

int bougerPeigne(int degre, int vitesse, int moteur);


#endif
