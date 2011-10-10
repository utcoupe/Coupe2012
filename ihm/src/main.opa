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

interface() =
    <h1>Interface Homme-Machine du robot 2012</h1>
    <>{Grid.get()}</>
    <div id=#control><h2>Bouton de control</h2>
    <button onclick={_ -> MyIrc.send_msg(asserv_stop, channel_asserv, true)}>[STOP]</button><br />
    <button onclick={_ -> MyIrc.send_msg(asserv_pause, channel_asserv, true)}>[PAUSE]</button><br />
    <button onclick={_ -> MyIrc.send_msg(asserv_resume, channel_asserv, true)}>[RESUME]</button><br />
    <button onclick={_ -> MyIrc.send_msg(asserv_recalB, channel_asserv, true)}>[RECAL BLUE]</button><button onclick={_ -> MyIrc.send_msg(asserv_recalR, channel_asserv, true)}>[RECAL RED]</button><br />
    </div>
    <br />
    <>{Compas.get()}</>
    <br />
    <div>
        BATTERIE STATUS : <br />
        {Batterie.show_bat()}
    </div>
    <br /><br /><br />
    <input id=#message onnewline={_ -> send_test()} value="ping" />
    <button onclick={_ -> send_test()}>Envoyer un message</button><br />
    <h2>Activité sur le channel <button onclick={_ -> Dom.transform([#messages <- <></>])}>Clear</button></h2>
    <div id=#messages onready={_ -> Network.add_callback((msg -> Dom.transform([#messages -<- msg])),mynetwork)}/>


server = Server.one_page_server("UTCOUPE BOT IHM", interface)
