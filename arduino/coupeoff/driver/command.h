#ifndef COMMAND_H_
#define COMMAND_H_

extern int consigneGauche, consigneDroite;

#include "message.h"

void cmd(int,int,int*,int); // from, id_cmd, *args, sizeArgs

#endif /* COMMAND_H_ */
