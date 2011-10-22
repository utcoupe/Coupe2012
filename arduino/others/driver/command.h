#ifndef COMMAND_H_
#define COMMAND_H_

#include "WProgram.h"
#include "message.h"
#include "pinceControl.h"
#include "sensor.h"
#include "tourelle.h"

void cmd(int,int,int*,int); // from, id_cmd, *args, sizeArgs

#endif /* COMMAND_H_ */
