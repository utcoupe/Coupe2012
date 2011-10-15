#include "Irc.h"

void ThreadFunction(void* UserData)
{
    while(true){
        ((IRCConnection * )UserData)->listen();
    }
}
