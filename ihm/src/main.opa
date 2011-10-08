
mynetwork = Network.cloud("utcoupe") : Network.network(xhtml)

send_test() = 
            msg = Dom.get_value(#message)
            do Irc_Asserv.send_msg(msg, true)
            Dom.clear_value(#message)

interface() =
    <h1>Interface Homme-Machine du robot 2012</h1>
    <>{bot_map_control()}</>
    <div id=#control><h2>Bouton de control</h2>
    <button onclick={_ -> Irc_Asserv.send_msg(asserv_stop,true)}>[STOP]</button><br />
    <button onclick={_ -> Irc_Asserv.send_msg(asserv_pause,true)}>[PAUSE]</button><br />
    <button onclick={_ -> Irc_Asserv.send_msg(asserv_resume,true)}>[RESUME]</button><br />
    <button onclick={_ -> Irc_Asserv.send_msg(asserv_recalB,true)}>[RECAL BLUE]</button><button onclick={_ -> Irc_Asserv.send_msg(asserv_recalR,true)}>[RECAL RED]</button><br />
    </div>
    <br />
    <>{compas()}</>
    <br /><br /><br /><br />
    <input id=#message onnewline={_ -> send_test()} value="ping" />
    <button onclick={_ -> send_test()}>Envoyer un message</button><br />
    <h2>Activité sur le channel <button onclick={_ -> Dom.transform([#messages <- <></>])}>Clear</button></h2>
    <div id=#messages onready={_ -> Network.add_callback((msg -> Dom.transform([#messages -<- msg])),mynetwork)}/>


server = Server.one_page_server("UTCOUPE BOT IHM", interface)
