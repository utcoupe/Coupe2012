#ifndef C_AX12_H_
#define C_AX12_H_

#include "message.h"
#include "ax12.h"
#include "Arduino.h"



#define NB_MOTEURS 4

extern AX12 motor[NB_MOTEURS];
extern int goal[NB_MOTEURS];
extern int ordre[NB_MOTEURS];

void cherche_moteurs(void);
int tourner(int id, int id_moteur, int position);
int get_position(int id, int id_moteur);
int lire(int id, int ordre, int id_moteur);

#endif /* C_AX12_H */
