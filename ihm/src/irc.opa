import stdlib.apis.irc

MyIrcLib(channel : string, 
         server : string, 
         username : string, 
         realname : string, 
         nickname : string,
         password : string,
         port : int,
         parse_message : string, string -> Parser.general_parser(void)) = 
{{
  
  connection = Mutable.make(none : option(Irc.connection))

  welcome(c : Irc.connection) =
      connection.set(Option.some(c))

  senderParser = parser
                  | ":" owner=([a-zA-Z0-9_]*) "!" .* -> Text.to_string(owner)
                 end

  handle_answer(_connection : Irc.connection, header : string, to : string, msg : string) =
    from = Option.default("error",Parser.try_parse(senderParser, header))
    myParser =
        parser
          | result={parse_message(from, to)} -> result
          | "ping" -> do Network.broadcast(<><strong>{from} {to}:</strong> ping <br/></>, mynetwork)
                      send_msg("pong", to, true)
          | "id" -> do Network.broadcast(<><strong>{from} {to}:</strong> id <br/></>, mynetwork)
                    send_msg(nickname, to, true)
          | msg=(.*) -> Network.broadcast(<><strong>{from} {to}:</strong> {msg} <br/></>, mynetwork)
         end
    Parser.parse(myParser, msg)

  @server @publish 
  send_msg(msg : string, chan: string, show : bool) = 
    c = connection.get()
    if Option.is_some(c) then
        do Irc.write_raw(Option.get(c), "PRIVMSG {chan} :{msg}\r\n")
        if show then 
            // Broadcasting for web client
            Network.broadcast(<><strong>{nickname} ({chan}):</strong> {msg} <br/></>, mynetwork)
        else 
            void
    else
        jlog("no connection")

  @server @publish
  join(chan : string) =
    c = connection.get()
    if Option.is_some(c) then
        Irc.write_raw(Option.get(c), "JOIN {chan}\r\n")
    else
        jlog("no connection")   

  run() = Irc.create_bot(username,  // username
                  realname,  // realname
                  nickname,      // nickname
                  password,         // password
                  server,
                  channel,
                  port,
                  welcome,
                  handle_answer)

}}

// CHANNEL ASSERV
position = Mutable.make({x=0 y=0 a=0})

batterie = Mutable.make({p=0.0 t=0.0})

MyIrc = MyIrcLib(channel_all, server_bot, 
         username, 
         realname, 
         nickname,
         password,
         port,
         parse_message)

do MyIrc.run()

ask_position() = 
    MyIrc.send_msg(asserv_position, channel_asserv, false)

ask_bat() =
    do MyIrc.send_msg("batterie p", channel_monitoring, false)
    do MyIrc.send_msg("batterie t", channel_monitoring, false)
    void

do Scheduler.sleep(10000,(-> Scheduler.timer(150, ask_position)))
do Scheduler.sleep(17000,(-> Scheduler.timer(10000, ask_bat)))

show_bat()=
    refresh()=
       b = batterie.get()
       Dom.transform([#bat <- <>Pourcentage : {b.p}%<br/>Time to : {b.t}h<br/></>])
    do Scheduler.sleep(18000,(-> Scheduler.timer(10000, refresh)))
    <div id=#bat />

Batterie = 
bat = Mutable.make({p=0.0 t=0.0})
{{
   set_p(p)=bat.set({bat.get() with ~p})
   set_t(t)=bat.set({bat.get() with ~t})
}}
