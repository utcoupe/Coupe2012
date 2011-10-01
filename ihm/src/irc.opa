import stdlib.apis.irc

channel_used = "#asserv"
channel_test = Mutable.make(none : option(Irc.connection))

say_hello(channel : string)(connection : Irc.connection) =
    do channel_test.set(Option.some(connection))
    Irc.write_raw(connection, "PRIVMSG {channel} : Hello \r\n")

senderParser = parser
                | ":" owner=([a-zA-Z0-9]*) "!" .* -> Text.to_string(owner)
               end

send_msg(msg : string) = 
    c = channel_test.get()
    if Option.is_some(c) then
        do Irc.write_raw(Option.get(c), "PRIVMSG {channel_used} :{msg}\r\n")
        Network.broadcast(<><strong>IHM :</strong> {msg} <br/></>, mynetwork)
    else
        jlog("no connection")

handle_answer(_connection : Irc.connection, header : string, to : string, msg : string) =
    do jlog("DEBUG : {header} {to} {msg}")
    //Privmsg(target : string, msg : string) = "PRIVMSG {target} :{msg} \r\n"
    from = Option.default("error",Parser.try_parse(senderParser, header))
    do Network.broadcast(<><strong>{from} :</strong> {msg} <br/></>, mynetwork)
    match msg with 
        | "ping" -> send_msg("pong")
        | "id" -> send_msg("ihm")
        | _ -> void
    //Irc.write_raw(connection, Privmsg(to, "{from} à écrit sur {to} : {message}"))

do Irc.create_bot("ihm",  // username
                  "IHM BOT",  // realname
                  "IHM",      // nickname
                  "",         // password
                  "10.42.43.94",
                  channel_used,
                  6667,
                  say_hello("#test"),
                  handle_answer)
