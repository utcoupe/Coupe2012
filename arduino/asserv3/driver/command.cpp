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

		case QA_TURN:
		{
			g_goal_a = (double)args[0] * DEG_TO_RAD;
			g_goal_x = g_observer.getX();
			g_goal_y = g_observer.getY();
			g_goal_type = GOAL_ANG;
			break;
		}

		case QA_GOTO:
		{
			g_goal_a = 0.0;
			g_goal_x = args[0] * ENC_MM_TO_TICKS;
			g_goal_y = args[1] * ENC_MM_TO_TICKS;
			g_goal_type = GOAL_POS;
			break;
		}

		case QA_STOP:
		{
			g_debug_on = 0;
			g_delta_regulator.stop();
			g_alpha_regulator.stop();
			break;
		}

		case QA_RESUME:
		{
			g_debug_on = 1;
			g_delta_regulator.resume();
			g_alpha_regulator.resume();
			break;
		}

		case QA_POS:
		{
			int x_mm = g_observer.mm_getX();
			int y_mm = g_observer.mm_getY();
			int a_deg = g_observer.deg_getA();
			int tab[] = {x_mm,y_mm,a_deg};
			sendMessage(id,tab,3);
			break;
		}

		case QA_RESET:
		{
			g_goal_type = GOAL_POS;
			g_debug_on = 0;
			g_goal_x = 0;
			g_goal_y = 0;
			g_goal_a = 0.0;
			g_observer.reset();
			g_delta_regulator.reset();
			g_alpha_regulator.reset();
			G_value_left_enc = 0;
			G_value_right_enc = 0;
			break;
		}
		
		default:
		{
			sendMessage(id,E_INVALID_CMD);
			break;
		}
	}
}
