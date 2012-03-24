#ifndef COMMAND_H_
#define COMMAND_H_

#include "WProgram.h"
#include "wiring.h"

#include "message.h"
#include "controle_ax12.h"
#include "percepteur.h"

void cmd(int,int,int*,int); // from, id_cmd, *args, sizeArgs

#endif /* COMMAND_H_ */
