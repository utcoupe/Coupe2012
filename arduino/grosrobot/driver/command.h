#ifndef COMMAND_H_
#define COMMAND_H_


#include "message.h"

#include "balais.h"
#include "tapis.h"
#include "idAX18.h"
#include "controleAX18.h"

void cmd(int,int,int*,int); // from, id_cmd, *args, sizeArgs

#endif /* COMMAND_H_ */
