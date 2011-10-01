
mynetwork = Network.cloud("utcoupe") : Network.network(xhtml)

send_test() = 
            do send_msg(Dom.get_value(#message))
            Dom.clear_value(#message)

interface() =
    <h1>Interface Homme-Machine du robot 2012</h1>
    <input id=#message value="ping" />
    <button onclick={_ -> send_test()}>Envoyer un message</button><br />
    <h2>Activité sur le channel <button onclick={_ -> Dom.transform([#messages <- <></>])}>Clear</button></h2>
    <div id=#messages onready={_ -> Network.add_callback((msg -> Dom.transform([#messages -<- msg])),mynetwork)}/>


server = Server.one_page_server("UTCOUPE BOT IHM", interface)
