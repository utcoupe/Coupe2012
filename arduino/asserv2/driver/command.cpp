#include "command.h"
#include "robot.h"
#include "parameters.h"
#include "message.h"
#include "encoder.h"
#include "Arduino.h"
#include "tools.h"
#include "fifo.h"


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

		case QA_GOTO:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				if (fifo.isEmpty() and robot.get_goal().type == G_POS)
					robot.update_speedf(convert_speed(args[2]));
				
				fifo.push(CMD_GOTO, args[0], args[1], args[2]);
				
				sendMessage(id, 1);
			}
			break;
		}

		case QA_GOTOR:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				if (fifo.isEmpty() and robot.get_goal().type == G_POS) {
					robot.update_speedf(convert_speed(args[2]));
				}
				
				fifo.push(CMD_GOTOR, args[0], args[1], args[2]);
				
				sendMessage(id, 1);
			}
			break;
		}

		case QA_TURN:
		{
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				fifo.push(CMD_TURN, args[0], args[1]);
				sendMessage(id, 1);
			}
			break;
		}

		case QA_TURNR:
		{
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				fifo.push(CMD_TURNR, args[0], args[1]);
				sendMessage(id, 1);
			}
			break;
		}

		case QA_PWM:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				fifo.push(CMD_PWM, args[0]);
			}
			break;
		}

		case QA_POS:
		{
			int x_mm = robot.get_x()*ENC_TICKS_TO_MM;
			int y_mm = robot.get_y()*ENC_TICKS_TO_MM;
			int a_deg = robot.get_a()*RAD_TO_DEG;
			int tab[] = {x_mm,y_mm,a_deg};
			sendMessage(id,tab,3);
	        break;
		}

		case QA_SET_POS:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				robot.set_x_y_a(mm_to_robotunit(args[0]),mm_to_robotunit(args[1]), degrees_to_robotunit(args[2]));
				sendMessage(id, 1);
			}
		}
		 
		case QA_CANCEL: 
		{
			robot.cancel();
			fifo.clear();
			sendMessage(id, 1);
			break;
		}

		case QA_GETENC:
		{
			int tab[2] = {value_left_enc,value_right_enc};
			sendMessage(id, tab, 2);
			break;
		}

		//
		//			CHANGEMENT DES COEFFS PID
		//

		// POSITION
		case QA_SET_PID_POSITION_D:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				robot.set_coeffs_pid_position_d((double)args[0]/100.0,(double)args[1]/100.0,(double)args[2]/100.0);
				sendMessage(id, 1);
			}
			break;
		}
		case QA_SET_PID_POSITION_A:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				robot.set_coeffs_pid_position_a((double)args[0]/100.0,(double)args[1]/100.0,(double)args[2]/100.0);
				sendMessage(id, 1);
			}
			break;
		}
		
		// ANGLE
		case QA_SET_PID_ANGLE_A:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				robot.set_coeffs_pid_angle_d((double)args[0]/100.0,(double)args[1]/100.0,(double)args[2]/100.0);
				sendMessage(id, 1);
			}
			break;
		}
		case QA_SET_PID_ANGLE_D:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				robot.set_coeffs_pid_angle_a((double)args[0]/100.0,(double)args[1]/100.0,(double)args[2]/100.0);
				sendMessage(id, 1);
			}
			break;
		}

		default:
		{
			sendMessage(id,E_INVALID_CMD);
			break;
		}
	}
}
