#ifndef MYIRCBOT_H_INCLUDED
#define MYIRCBOT_H_INCLUDED

#include <pircbot/all.h>
#include <string>
using namespace pircbot;
using namespace std;

/**
 * Our custom bot,
 * called MyCIrcBot.
 *
 * Inherit from PircBot
 * and implement the methods
 * you need to utilize.
 */
class MyCIrcBot : public PircBot
{
	public:

		/**
		 * Default constructor
		 * in which we set the name
		 * of our bot.
		 */
		MyCIrcBot();

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
		   throw(Exception &);
};



#endif // MYIRCBOT_H_INCLUDED
