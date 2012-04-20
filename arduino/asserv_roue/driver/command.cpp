#include "command.h"
#include "parameters.h"
#include "message.h"
#include "Arduino.h"






double G_consign = 0;
int G_debug_on = 0;


PositionObserver observerG, observerD;
PosAndSpeedRegulator regulatorG, regulatorD;

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

		case QTEST_ID: /* Identification */
		{
			sendMessage(id, ID_ASSERV, (char*)"asserv");
			break;
		}

		case QTEST_PING:
		{
			sendMessage(id, (char*)"Pong");
			break;
		}

		case QTEST_DEBUG:
		{
			G_debug_on = args[0];
			break;
		}

		case QTEST_SPEED:
		{
			regulatorG.setMod(PosAndSpeedRegulator::SPEED);
			regulatorD.setMod(PosAndSpeedRegulator::SPEED);
			regulatorG.reset();
			regulatorD.reset();
			// 100 * mm/s to ticks/cycle
			G_consign = (((double)args[0]) * (double)ENC_MM_TO_TICKS * (double)DUREE_CYCLE / (double)1000);
			Serial.print("G_consign "); Serial.println(G_consign);
			sendMessage(id, (char*)"1");
			break;
		}

		case QTEST_POSITION:
		{
			regulatorG.setMod(PosAndSpeedRegulator::BOTH);
			regulatorD.setMod(PosAndSpeedRegulator::BOTH);
			G_consign = (((double)args[0]) * (double)ENC_MM_TO_TICKS);
			Serial.print("G_consign "); Serial.println(G_consign);
			sendMessage(id, (char*)"1");
			break;
		}

		case QTEST_SETVPID:
		{
			regulatorG.setSpeedPID(((double)args[0]) / (double) 10, ((double)args[1]) / (double) 10, ((double)args[2]) / (double) 10);
			regulatorD.setSpeedPID(((double)args[0]) / (double) 10, ((double)args[1]) / (double) 10, ((double)args[2]) / (double) 10);
			sendMessage(id, args, 3);
			break;
		}

		case QTEST_SETPPID:
		{
			regulatorG.setPosPID(((double)args[0]) / (double) 100, ((double)args[1]) / (double) 100, ((double)args[2]) / (double) 100);
			regulatorD.setPosPID(((double)args[0]) / (double) 100, ((double)args[1]) / (double) 100, ((double)args[2]) / (double) 100);
			sendMessage(id, args, 3);
			break;
		}

		case QTEST_STOP:
		{
			regulatorG.stop();
			regulatorD.stop();
			G_debug_on = 0;
			break;
		}

		case QTEST_RESUME:
		{
			regulatorG.resume();
			regulatorD.resume();
		}

		default:
		{
			sendMessage(id,E_INVALID_CMD);
			break;
		}
	}
}
