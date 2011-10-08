import stdlib.apis.irc

channel_used = "#asserv"
channel_test = Mutable.make(none : option(Irc.connection))
position = Mutable.make({x=0 y=0 a=0})

say_hello(channel : string)(connection : Irc.connection) =
    do channel_test.set(Option.some(connection))
    do Scheduler.timer(190, ask_position)
    Irc.write_raw(connection, "PRIVMSG {channel} : Hello \r\n")

senderParser = parser
                | ":" owner=([a-zA-Z0-9]*) "!" .* -> Text.to_string(owner)
               end

@server @publish send_msg(msg : string, show : bool) = 
    c = channel_test.get()
    if Option.is_some(c) then
        do Irc.write_raw(Option.get(c), "PRIVMSG {channel_used} :{msg}\r\n")
        if show then Network.broadcast(<><strong>IHM :</strong> {msg} <br/></>, mynetwork)
        else void
    else
        jlog("no connection")

parse_message(from) : Parser.general_parser(void) = 
    parser
      | "ping" -> do Network.broadcast(<><strong>{from} :</strong> ping <br/></>, mynetwork)
                  send_msg("pong", true)
      | "id" -> do Network.broadcast(<><strong>{from} :</strong> id <br/></>, mynetwork)
                send_msg("ihm", true)
      | "-1.0." x=Rule.integer "." y=Rule.integer "." a=Rule.integer -> position.set({~x ~y ~a})
      | msg=(.*) -> Network.broadcast(<><strong>{from} :</strong> {msg} <br/></>, mynetwork)
    end

handle_answer(_connection : Irc.connection, header : string, to : string, msg : string) =
    from = Option.default("error",Parser.try_parse(senderParser, header))
    Parser.parse(parse_message(from), msg)

ask_position() = 
    send_msg("position", false)

do Irc.create_bot("ihm",  // username
                  "IHM BOT",  // realname
                  "IHM",      // nickname
                  "",         // password
                  "10.42.43.94",
                  channel_used,
                  6667,
                  say_hello("#test"),
                  handle_answer)
