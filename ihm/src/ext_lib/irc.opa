/*
 * @author Matthieu Guffroy
 * Ce fichier est une extension à la lib irc
 */

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
          | "ping" -> do Network.broadcast(<><strong>{from} {to}:</strong> ping <br/></>, Network.cloud("utcoupe"))
                      send_msg("pong", to, true)
          | "id" -> do Network.broadcast(<><strong>{from} {to}:</strong> id <br/></>, Network.cloud("utcoupe"))
                    send_msg(nickname, to, true)
          | msg=(.*) -> Network.broadcast(<><strong>{from} {to}:</strong> {msg} <br/></>, Network.cloud("utcoupe"))
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

