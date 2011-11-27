import stdlib.widgets.slider

mynetwork = Network.cloud("utcoupe") : Network.network(xhtml)

send_test() = 
            msg = Dom.get_value(#message)
            do MyIrc.send_msg(msg,  channel_all, true)
            Dom.clear_value(#message)

debug() = 
        do jlog(Debug.dump(Position.get()))
        do jlog(Debug.dump(Batterie.get()))
        void

server = Server.one_page_server("UTCOUPE BOT IHM", Interface.run)
