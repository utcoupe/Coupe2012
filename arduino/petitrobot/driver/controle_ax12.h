#ifndef C_AX12_H_
#define C_AX12_H_

#include "message.h"
#include "ax12.h"
#include "Arduino.h"



#define NB_MOTEURS 4

extern AX12 motor[NB_MOTEURS];

void cherche_moteurs(void);
byte tourner(int id, int id_moteur, int position);

#endif /* C_AX12_H */