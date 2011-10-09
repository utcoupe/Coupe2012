/*
 * @author Matthieu Guffroy
 * 
 */

/*
 * Dans ce fichier se trouve l'initialisation de la connexion IRC.
 * On retrouve égallement toutes les données du robot extraites des requetes irc (ex: position, batterie...)
 */

Position =
pos = Mutable.make({x=0 y=0 a=0})
{{
   ask()=MyIrc.send_msg(asserv_position, channel_asserv, false)

   set(p)=pos.set(p)

   get()=pos.get()
}}

Batterie = 
bat = Mutable.make({p=0.0 t=0.0})
{{
   ask()= 
      do MyIrc.send_msg(monitoring_bat_p, channel_monitoring, false)
      do MyIrc.send_msg(monitoring_bat_t, channel_monitoring, false)
      void

   set_p(p)=bat.set({bat.get() with ~p})

   set_t(t)=bat.set({bat.get() with ~t})
  
   get() = bat.get()

   show_bat()=
      refresh()=
         b = get()
         Dom.transform([#bat <- <>* {Math.round(b.p)}% Restant<br/> * {b.t}h Restant<br/></>])
      do Scheduler.timer(10000, refresh)
      <div id=#bat />
}}
// Wait 10 sec and ask every 150ms the position, 
// and every 10sec(TODO augment this value when test will be finished) the battery state

do Scheduler.sleep(10000,(-> Scheduler.timer(150, Position.ask)))
do Scheduler.sleep(10000,(-> Scheduler.timer(10000, Batterie.ask)))


// PARSER DE Reception des messages
parse_message(from, to): Parser.general_parser(void) =
    if to == channel_asserv then
        if from == asserv_botname then
            parser
             | "-1.0." x=Rule.integer "." y=Rule.integer "." a=Rule.integer -> Position.set({~x ~y ~a})
            end
        else
            do jlog("from : {from}, to : {to}")
            parser
             | Rule.fail -> void
            end
    else if to == channel_monitoring then
        if from == monitoring_botname then
            parser
             | p=Rule.float "%" -> Batterie.set_p(p)
             | t=Rule.float -> Batterie.set_t(t)
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

// Connection IRC, tout les parametres sont appelés du fichier config.opa
MyIrc = MyIrcLib(channel_all, server_bot, 
         username, 
         realname, 
         nickname,
         password,
         port,
         parse_message)
// Launch IRC connection
do MyIrc.run()


