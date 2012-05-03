#include "command.h"
#include "parameters.h"
#include "message.h"
#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#	include "wiring.h"
#endif

#include "globals.h"
#include "goal.h"
#include "tools.h"

#include "control.h"



/**
 * Analyse le message et effectue les actions associees
 *
 * @param id : l'identifiant associe au message
 * @param header : le type de message (en-tete)
 * @param args : le tableau d'entier contenant les arguments
 * */
void cmd(int id, int id_cmd, int* args, int size){
	/* On analyse le message en fonction de son type */
	switch(id_cmd){

		case QA_ID: /* Identification */
		{
			sendMessage(id, ID_ASSERV, (char*)"asserv");
			break;
		}

		case QA_PING:
		{
			sendMessage(id, (char*)"Pong");
			break;
		}

		case QA_DEBUG:
		{
			g_debug_on = args[0];
			break;
		}

		case QA_GETENC:
		{
			/*Serial.print(g_left_encoder.getValue());
			Serial.print(" ");
			Serial.println(g_right_encoder.getValue());
			* */
			Serial.print(G_value_left_enc);
			Serial.print(" ");
			Serial.println(G_value_right_enc);
			break;
		}

		case QA_SETPID:
		{
			g_delta_regulator.setPosPID(args[0]/1000.0,args[1]/1000.0,args[2]/1000.0);
			break;
		}

		case QA_DUMP:
		{
			dump();
			break;
		}

		case QA_TURN:
		{
			g_debug_on = 1;
			Goal::get()->set(
				Goal::GOAL_ANG,
				g_observer.getX(),
				g_observer.getY(),
				deg_to_rad(args[0]),
				deg_s_to_rad_cycle(args[1]),
				0.0,
				id,
				false
			);
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			g_delta_regulator.setMod(PosAndSpeedRegulator::BOTH);
			g_alpha_regulator.setMod(PosAndSpeedRegulator::BOTH);
			sendMessage(id, 1);
			break;
		}

		case QA_GOTO:
		{
			g_debug_on = 1;
			Goal::get()->set(
				Goal::GOAL_POS,
				mm_to_ticks(args[0]),
				mm_to_ticks(args[1]),
				0.0,
				0.0,
				mm_s_to_ticks_cycle(args[2]),
				id,
				false
			);
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			g_delta_regulator.setMod(PosAndSpeedRegulator::BOTH);
			g_alpha_regulator.setMod(PosAndSpeedRegulator::BOTH);
			sendMessage(id, 1);
			break;
		}

		case QA_DSPEED:
		{
			g_debug_on = 1;
			Goal::get()->set(
				Goal::GOAL_DELTA_SPEED,
				(TICKS)0,
				(TICKS)0,
				0.0,
				mm_s_to_ticks_cycle(args[0]),
				0.0,
				id
			);
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			g_delta_regulator.setMod(PosAndSpeedRegulator::SPEED);
			g_alpha_regulator.setMod(PosAndSpeedRegulator::NONE);
			break;
		}

		case QA_ASPEED:
		{
			g_debug_on = 1;
			Goal::get()->set(
				Goal::GOAL_ALPHA_SPEED,
				(TICKS)0,
				(TICKS)0,
				0.0,
				0.0,
				deg_s_to_rad_cycle(args[0]),
				id
			);
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			g_delta_regulator.setMod(PosAndSpeedRegulator::NONE);
			g_alpha_regulator.setMod(PosAndSpeedRegulator::SPEED);
			break;
		}

		case QA_PWM:
		{
			Goal::get()->set(
				Goal::GOAL_PWM,
				args[0],
				args[1],
				0.0,
				0.0,
				0.0,
				id
			);
			break;
		}

		case QA_STOP:
		{
			g_debug_on = 0;
			g_delta_regulator.stop();
			g_alpha_regulator.stop();
			sendMessage(id, 0);
			break;
		}

		case QA_RESUME:
		{
			g_debug_on = 1;
			g_delta_regulator.resume();
			g_alpha_regulator.resume();
			sendMessage(id, 0);
			break;
		}

		case QA_CANCEL:
		{
			// TODO
			break;
		}

		case QA_RESET:
		{
			Goal::get()->reset();
			g_debug_on = 0;
			g_observer.reset();
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			G_value_left_enc = 0;
			G_value_right_enc = 0;
			sendMessage(id, 0);
			break;
		}

		case QA_SET_POS:
		{
			g_observer.setPos(
				mm_to_ticks(args[0]),
				mm_to_ticks(args[1]),
				deg_to_rad(args[2])
			);
			sendMessage(id, 0);
			break;
		}

		case QA_POS:
		{
			int tab[3] = {(int)g_observer.mm_getX(), (int)g_observer.mm_getY(), (int)g_observer.deg_getA()};
			sendMessage(id, tab, 3);
			break;
		}
		
		default:
		{
			sendMessage(id,E_INVALID_CMD);
			break;
		}
	}
}
