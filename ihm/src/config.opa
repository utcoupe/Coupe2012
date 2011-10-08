/*
  Ce fichier contient toute les variables de config "importante"
*/

// CONFIG IRC
server_bot = "10.42.43.94"
username = "Ihm"
realname = "Ihm Bot"
nickname = "IHM"
password = ""
port = 6667

// CONFIG ASSERV
channel_asserv = "#asserv"
// commandes
asserv_stop = "stop"
asserv_pause = "pause"
asserv_resume = "resume"
asserv_recalB = "recal 2"
asserv_recalR = "recal 1"
asserv_position = "position"
asserv_angle = "angle"
// Reception d'une position
parse_message_asserv: Parser.general_parser(void) = 
    parser
      | "-1.0." x=Rule.integer "." y=Rule.integer "." a=Rule.integer -> position.set({~x ~y ~a})
    end
