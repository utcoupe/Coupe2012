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
asserv_botname = "asservbot"

// CONFIG MONITORING
channel_monitoring = "#monitoring"
monitoring_botname = "monitoringbot"

// Reception d'un message
parse_message(from, to): Parser.general_parser(void) =
    if to == channel_asserv then
        if from == asserv_botname then
            parser
             | "-1.0." x=Rule.integer "." y=Rule.integer "." a=Rule.integer -> position.set({~x ~y ~a})
            end
        else
            do jlog("from : {from}, to : {to}")
            parser
             | Rule.fail -> void
            end
    else if to == channel_monitoring then
        if from == monitoring_botname then
            parser
             | p=Rule.float "%" -> batterie.set({batterie.get() with ~p})
             | t=Rule.float -> batterie.set({batterie.get() with ~t})
            end
        else
            do jlog("from : {from}, to : {to}")
            parser
             | Rule.fail -> void
            end
      else
         parser
          | Rule.fail -> void
         end
            
