#include "command.h"
#include "parameters.h"
#include "robotstate.h"
#include "message.h"
#include "encoder.h"
#include "pwm.h"
#include "include_arduino.h"
#include "globals.h"
#include "goal.h"
#include "tools.h"


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
			int tab[3] = {(int)g_observer.mm_x(), (int)g_observer.mm_y(), (int)g_observer.deg_a()};
			sendMessage(id, tab, 3);
			break;
		}
		
		case QA_PWM:
		{
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
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
				sendMessage(id, 1);
			}
			break;
		}
		
		case QA_STOP:
		{
			g_debug_on = 0;
			g_delta_regulator.stop();
			g_alpha_regulator.stop();
			g_stop = true;
			sendMessage(id, 0);
			break;
		}

		case QA_RESUME:
		{
			g_debug_on = 1;
			g_delta_regulator.resume();
			g_alpha_regulator.resume();
			g_stop = false;
			sendMessage(id, 0);
			break;
		}

		case QA_CANCEL:
		{
			// TODO
			sendMessage(id, 0);
			break;
		}

		case QA_RESET:
		{
			Goal::get()->reset();
			g_debug_on = 0;
			g_observer.reset();
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			value_left_enc = 0;
			value_right_enc = 0;
			sendMessage(id, 0);
			break;
		}

		case QA_GETENC:
		{
			int tab[2] = {value_left_enc,value_right_enc};
			sendMessage(id, tab, 2);
			break;
		}

		default:
		{
			sendMessage(id,E_INVALID_CMD);
			break;
		}
	}
}
