#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "protocole.h"

#define SERIAL_BAUD 115200



void initSerialLink();
void readIncomingData();
void sendMessage(int id, int i);
void sendMessage(int id, char* str);
void sendMessage(int id, int i, char* s);
void sendMessage(int id, char* str, int i);
void sendMessage(int id, int *tabi, int size);
void sendMessage(int id, char** tabs, int nbStr, int *tabi, int nbInt);
void sendMessage(int id, int* tabi, int nbInt, char** tabs, int nbStr);


#endif /* MESSAGE_H_ */
