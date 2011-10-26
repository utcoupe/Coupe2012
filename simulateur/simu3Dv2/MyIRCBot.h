#ifndef MYIRCBOT_H_INCLUDED
#define MYIRCBOT_H_INCLUDED

#include <pircbot/all.h>
#include <string>
using namespace pircbot;
using namespace std;

/**
 * Our custom bot,
 * called HackBotcpp.
 *
 * Inherit from PircBot
 * and implement the methods
 * you need to utilize.
 */
class HackBotcpp : public PircBot
{
	public:

		/**
		 * Default constructor
		 * in which we set the name
		 * of our bot.
		 */
		HackBotcpp()
		{
			this->setName("HackBotcpp");
		}

		/**
		 * We override the onMessage here
		 * so that we responde to someone
		 * saying "time".
		 *
		 * @param channel The channel to which the message was sent.
		 * @param sender The nick of the person who sent the message.
		 * @param login The login of the person who sent the message.
		 * @param hostname The hostname of the person who sent the message.
		 * @param message The actual message sent to the channel.
		 *
 		 * @throws Exception Inherit from Exception and throw any type
		 * of exception you would like.
		 */
		void onMessage(const char * const channel,
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
};



#endif // MYIRCBOT_H_INCLUDED
