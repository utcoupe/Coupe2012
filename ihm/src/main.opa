
mynetwork = Network.cloud("utcoupe") : Network.network(xhtml)

history = Mutable.make({l = ["ping","help","id"] : list(string); pos = 4})

send_test() = 
            msg = Dom.get_value(#message)
            do send_msg(msg, true)
            h = history.get()
            do history.set({h with l=List.add(msg, h.l) ; pos=List.length(h.l)+1})
            Dom.clear_value(#message)

keyup(e) = 
        if Option.default(-1,e.key_code) == Dom.Key.UP then
        h = history.get()
        do Dom.set_value(#message, Option.default("",List.get(h.pos-1, h.l)))
        history.set({h with pos=h.pos-1})

keydown(e) = 
        if Option.default(-1, e.key_code) == Dom.Key.DOWN then
        h = history.get()
        do Dom.set_value(#message, Option.default("",List.get(h.pos+1, h.l)))
        history.set({h with pos=h.pos+1})

interface() =
    <h1>Interface Homme-Machine du robot 2012</h1>
    <>{bot_map_control()}</>
    <div id=#control><h2>Bouton de control</h2>
    <button onclick={_ -> send_msg("stop",true)}>[STOP]</button><br />
    <button onclick={_ -> send_msg("pause",true)}>[PAUSE]</button><br />
    <button onclick={_ -> send_msg("resume",true)}>[RESUME]</button><br />
    <button onclick={_ -> send_msg("recal 2",true)}>[RECAL BLUE]</button><button onclick={_ -> send_msg("recal 1",true)}>[RECAL RED]</button><br />
    </div>
    <br />
    <>{compas()}</>
    <br /><br /><br />
    <input id=#message onnewline={_ -> send_test()} onkeyup={e -> keyup(e)} onkeydown={e -> keydown(e)} value="ping" />
    <button onclick={_ -> send_test()}>Envoyer un message</button><br />
    <h2>Activité sur le channel <button onclick={_ -> Dom.transform([#messages <- <></>])}>Clear</button></h2>
    <div id=#messages onready={_ -> Network.add_callback((msg -> Dom.transform([#messages -<- msg])),mynetwork)}/>


server = Server.one_page_server("UTCOUPE BOT IHM", interface)
