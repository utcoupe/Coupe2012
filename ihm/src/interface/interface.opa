/*
 * @author Matthieu Guffroy
 *
*/
/*
  Interface de l'application
*/

// CALL BOOTSTRAP
import stdlib.themes.bootstrap
import stdlib.widgets.bootstrap
WB = WBootstrap

Interface = {{

    topbar =
        WB.Navigation.topbar(
            WB.Layout.fixed(
                WB.Navigation.brand(<>UTCOUPE 2012</>, none, ignore) <+>
                WB.Navigation.nav([
                    {inactive=<>Accueil</> href=none onclick=load(welcome())},
                    {inactive=<>Configuration</> href=none onclick=load(configuration())},
                    {inactive=<>Map</> href=none onclick=load(map())}
                ])
            )
        )

    load(content : xhtml) = _ -> Dom.transform([#content <- content])

    welcome() = 
         WB.Div.content(
            WB.Div.page_header(1, "UTCOUPE IHM", some("Interface Homme - Machine")) <+>
            WB.Grid.row([
                {third=1 offset=none content=<><h2>Gros robot</h2></>},
                {third=1 offset=none content=<><h2>Petit robot</h2></>},
                {third=1 offset=none content=<><h2>Balise</h2></>}
            ])
        )

    configuration() = 
         WB.Div.content(
            WB.Div.page_header(1, "Configuration", some("Configuration des robots")) <+>
            WB.Grid.row([
                {span=10 offset=none content=<><h2>Paramétres</h2></>},
                {span=4 offset=none content=<><h3>Robots</h3></>}
            ])
        )        

    map() =
         WB.Div.content(
            WB.Div.page_header(1, "Carte", some("Visualisation des robots")) <+>
            WB.Grid.row([
                {span=10 offset=none content=<><h2>Map</h2>{Grid.get()}</>},
                {span=4 offset=none content=<><h3>Controle</h3></>}
            ])
        )  

    run() =
        WB.Layout.fixed(
            topbar <+>
            <br /><br /><br />
            <div id=#content>{welcome()}</div>
        )

/*
    {WB.Layout.fixed(
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
    <div id=#messages onready={_ -> Network.add_callback((msg -> Dom.transform([#messages -<- msg])),mynetwork)}/>)}</>
*/

}}
