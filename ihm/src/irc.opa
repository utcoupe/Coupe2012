import stdlib.apis.irc

MyIrcLib(channel : string, 
         server : string, 
         username : string, 
         realname : string, 
         nickname : string,
         password : string,
         port : int,
         parse_message : Parser.general_parser(void)) = 
{{
  
  connection = Mutable.make(none : option(Irc.connection))

  welcome(c : Irc.connection) =
      connection.set(Option.some(c))

  senderParser = parser
                  | ":" owner=([a-zA-Z0-9_]*) "!" .* -> Text.to_string(owner)
                 end

  handle_answer(_connection : Irc.connection, header : string, _to : string, msg : string) =
    from = Option.default("error",Parser.try_parse(senderParser, header))
    myParser =
        parser
          | result={parse_message} -> result
          | "ping" -> do Network.broadcast(<><strong>{from} :</strong> ping <br/></>, mynetwork)
                      send_msg("pong", true)
          | "id" -> do Network.broadcast(<><strong>{from} :</strong> id <br/></>, mynetwork)
                    send_msg(nickname, true)
          | msg=(.*) -> Network.broadcast(<><strong>{from} :</strong> {msg} <br/></>, mynetwork)
         end
    Parser.parse(myParser, msg)

  @server @publish 
  send_msg(msg : string, show : bool) = 
    c = connection.get()
    if Option.is_some(c) then
        do Irc.write_raw(Option.get(c), "PRIVMSG {channel} :{msg}\r\n")
        if show then 
            // Broadcasting for web client
            Network.broadcast(<><strong>{nickname} :</strong> {msg} <br/></>, mynetwork)
        else 
            void
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

Irc_Asserv = MyIrcLib(channel_asserv, server_bot, 
         username, 
         realname, 
         nickname,
         password,
         port,
         parse_message_asserv)

do Irc_Asserv.run()

ask_position() = 
    Irc_Asserv.send_msg(asserv_position, false)

do Scheduler.sleep(10000,(-> Scheduler.timer(150, ask_position)))
