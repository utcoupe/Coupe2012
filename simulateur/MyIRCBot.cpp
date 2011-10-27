
#include "MyIRCBot.h"



MyIrcBot::MyIrcBot()
{
    this->setName("MyIrcBot");
}


void MyIrcBot::onMessage(const char * const channel,
            const char * const sender,
            const char * const login,
            const char * const hostname,
            const char * const message)
   throw(Exception &)
{
    //The message that someone from the server said.
    string messageString(message);

    //Create a string of the sender of the message from
    //the sender *char
    string senderString(sender);

    //Look to see if the message string is equal to time
    if(messageString == "time")
    {
        //Create our time string and sppend the name
        //of the person asking for the time to it.
        string time = "Time for you to get a watch";
        senderString += ": The time is now ";
        senderString += time;

        //Okay, so send to the channel our message
        sendMessage(channel, senderString.c_str());
    }
}
