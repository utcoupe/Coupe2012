#include "command.h"
#include "robot.h"
#include "parameters.h"
#include "message.h"
#include "encoder.h"
#include "WProgram.h"

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
				robot.go_to(
					(double)args[0] * ENC_MM_TO_TICKS,
					(double)args[1] * ENC_MM_TO_TICKS,
					(double)args[2] * ENC_MM_TO_TICKS / 1000.0
				);
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
				double co = cos(robot.get_a());
				double si = sin(robot.get_a());

				long int dx = (long int)(((double)args[0]*co - (double)args[1]*si) * ENC_MM_TO_TICKS);
				long int dy = (long int)(((double)args[0]*si + (double)args[1]*co) * ENC_MM_TO_TICKS);
				
				robot.go_to(
					robot.get_x() + dx,
					robot.get_y() + dy,
					(double)args[2] * ENC_MM_TO_TICKS / 1000.0
				);
				
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
				double angle = fmod(((double)args[0]) * DEG_TO_RAD, M_PI);
				robot.turn(angle,args[1] * ENC_MM_TO_TICKS / 1000.0);
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
				double angle = fmod(((double)args[0]) * DEG_TO_RAD + robot.get_a(), M_PI);
				robot.turn(angle,args[1] * ENC_MM_TO_TICKS / 1000.0);
				sendMessage(id, 1);
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

		/*case QA_MCALIB: //TODO a eclater en calibration manuel de l'angle ,de x et de y
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				pushGoalManualCalibration(TYPE_CALIB_X, (double)args[0]*ENC_MM_TO_TICKS);
				pushGoalManualCalibration(TYPE_CALIB_Y, (double)args[1]*ENC_MM_TO_TICKS);
				pushGoalManualCalibration(TYPE_CALIB_ANGLE, (double)args[2]*DEG_TO_RAD);
				sendMessage(id, 1);
			}
			break;
		}

		case QA_ACALIB:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				if(args[0] == 1){
					pushGoalAutoCalibration(id,true);
				}
				else{
					pushGoalAutoCalibration(id,false);
				}

				sendMessage(id, 1);
			}
			break;
		}

		case QA_DELAY:
		{
			if (size < 1)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				pushGoalDelay(args[0]);
				sendMessage(id, 1);
			}
			break;
		}

		case QA_PWM:
		{
			if (size < 2)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				sendMessage(id, 1);
				pushGoalPwm(id,args[0],args[1]);
			}
			break;
		}
		/*
		case Q_MODIF_GOAL_ABS:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				current_goal.type = TYPE_POSITION;
				current_goal.isReached = false;
				current_goal.x = (double)args[0]*ENC_MM_TO_TICKS;
				current_goal.y = (double)args[1]*ENC_MM_TO_TICKS;
				current_goal.speed = args[2];
				sendMessage(id, 1);
			}
			break;
		}

		case Q_MODIF_GOAL_REL:
		{
			if (size < 3)
				sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
			else
			{
				double co = cos(robot_state.angle);
				double si = sin(robot_state.angle);

				current_goal.type = TYPE_POSITION;
				current_goal.isReached = false;
				current_goal.x = (args[0]*co-args[1]*si)*18+robot_state.x;
				current_goal.y = (args[0]*si+args[1]*co)*18+robot_state.y;
				current_goal.speed = args[2];
				sendMessage(id, 1);
			}
			break;
		}
		 */
		/*case QA_CANCEL: 
		{
			clearGoals();
			current_goal.isCanceled = true;
			sendMessage(id, 1);
			break;
		}

		case QA_STOP: 
		{
			current_goal.isPaused = true;
			sendMessage(id, 1);
			break;
		}

		case QA_RESUME: 
		{
			current_goal.isPaused = false;
			sendMessage(id, 1);
			break;
		}

		case QA_GETSENS:
		{
			if (value_pwm_right > 0)
				sendMessage(id, AVANT);
			else if (value_pwm_right < 0)
				sendMessage(id, ARRIERE);
			else
				sendMessage(id, ARRET);
			break;
		}
		*/

		case QA_GETENC:
		{
			int tab[2] = {value_left_enc,value_right_enc};
			sendMessage(id, tab, 2);
			break;
		}

		case Q_DEBUG : //TODO a degager quand tout marche
		{
			/*Serial.print("?,_________________§");
			Serial.print("uptime: ");Serial.print(millis());
			Serial.print("§angle: ");Serial.print(robot_state.angle, DEC);
			Serial.print("§speed: ");Serial.print(robot_state.speed*ENC_TICKS_TO_MM, DEC);
			Serial.print("§speedt: ");Serial.print(robot_state.speed, DEC);
			Serial.print("§x: ");Serial.print(robot_state.x*ENC_TICKS_TO_MM, DEC);
			Serial.print("§xt: ");Serial.print(robot_state.x, DEC);
			Serial.print("§y: ");Serial.print(robot_state.y*ENC_TICKS_TO_MM, DEC);
			Serial.print("§yt: ");Serial.print(robot_state.y, DEC);
			Serial.print("§encL: ");Serial.print(value_left_enc);
			Serial.print("§encR: ");Serial.println(value_right_enc);*/
			break;
		}
		

		default:
		{
			sendMessage(id,E_INVALID_CMD);
			break;
		}
	}
}
