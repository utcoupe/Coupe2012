#include "command.h"

/* Analyse le message et effectue les actions associees
 *      <> id : l'identifiant associe au message
 *      <> header : le type de message (en-tete)
 *      <> args : le tableau d'entier contenant les arguments
 * */

void motor_send_message(int id_msg, int id_moteur, int type, int valeur) {

}

void start_buldozer(int id) {

}

void stop_buldozer(int id) {

}

void sortir_balais(int id) {

}

void rentrer_balais(int id) {

}

// from, id_cmd, *args, sizeArgs
void cmd(int id, int header, int *args, int size){
  /* On analyse le message en fonction de son type */
  switch(header)
    {
    case Q_BULDOZER_START:
      {
        if (size < 1)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          {
            start_buldozer(args[0]);
          }
        break;
      }
    case Q_BULDOZER_STOP:
      {
        if (size < 1)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          {
            stop_buldozer(args[0]);
          }
        break;
      }
    case Q_SORTIR_BALAIS:
      {
        if (size < 1)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          {
            sortir_balais(args[0]);
          }
        break;
      }
    case Q_RENTRER_BALAIS:
      {
        if (size < 1)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          {
            rentrer_balais(args[0]);
          }
        break;
      }
    default:
      {
        sendMessage(id,-1);
        break;
      }
    }
}
