#include "command.h"
#include "protocole.h"

/* Analyse le message et effectue les actions associees
 *      <> id : l'identifiant associe au message
 *      <> header : le type de message (en-tete)
 *      <> args : le tableau d'entier contenant les arguments
 * */

// from, id_cmd, *args, sizeArgs
void cmd(int id, int header, int *args, int size){
  /* On analyse le message en fonction de son type */
  switch(header)
    {
    case Q_PING:
      {
        if (size < 1)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          {
            sendMessage(id, args[0]);
          }
        break;
      }
    case Q_TOURNER:
      {
        if (size < 2)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          {
            sendMessage(id, tourner(id, args[0], args[1]));
          }
        break;
      }
    case Q_GET_POS:
      {
        if (size < 1)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          sendMessage(id, get_position(id, args[0]));
        break;
      }
    case Q_LIRE:
      {
        if (size < 2)
          sendMessage(id, E_INVALID_PARAMETERS_NUMBERS);
        else
          sendMessage(id, lire(id, args[0], args[1]));
        break;
      }
    case Q_OUVRIR_PEIGNES:
      {
        motor[0].writeInfo (MOVING_SPEED, 500);
        motor[2].writeInfo (MOVING_SPEED, 500);
        tourner(id, 0, -70);
        tourner(id, 2, 70);
        break;
      }
    case Q_FERMER_PEIGNES:
      {
        motor[0].writeInfo (MOVING_SPEED, 200);
        motor[2].writeInfo (MOVING_SPEED, 200);
        tourner(id, 0, 150);
        tourner(id, 2, -150);
        break;
      }
    default:
      {
        sendMessage(id,-1);
        break;
      }
    }
}
