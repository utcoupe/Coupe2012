/*
 * @author Matthieu Guffroy
 *
*/
/*
  Représentation du terrain et du robot.
  A base de canvas.
*/
Grid = {{

    id = "map_bot"
    canvas_width=Int.of_float(conf_map.size*conf_map.width)
    canvas_height=Int.of_float(conf_map.size*conf_map.height)
    canvas = MyCanvas(id,canvas_width,canvas_height)

    // Cette fonction dessine la map
    // Pour l'instant c'est un rectangle gris, mais a terme il faudrait dessiner
    // ou importer une image représentant mieux le terrain.
    draw_map() =
        canvas.rectangle(Color.lightgrey,0,0,canvas_width,canvas_height)

    // Cette fonction dessine le robot
    draw_bot(x,y,a)=
        canvas.arc(Color.black, f2i(i2f(x)*conf_map.size),f2i(i2f(y)*conf_map.size),f2i(conf_map.size*conf_map.robot_rayon), a, 0.2)

    // This function is called when canvas is loaded
    on_load(_)=
        Scheduler.timer(200, (-> pos = Position.get()
                                 do draw_map()
                                 do draw_bot(pos.x,
                                             pos.y,
                                             pos.a)
                                 void))
    
    // This function is called when someone click on the map
    on_click(ev)=
        //position en pixel
        pos_px=Dom.Dimension.sub(ev.mouse_position_on_page,Dom.get_offset(#{id}))
        //position en mm
        pos_mm={x=f2i(i2f(pos_px.x_px)/conf_map.size) 
                y=f2i(i2f(canvas_height) - i2f(pos_px.y_px)/conf_map.size)}
        MyIrc.send_msg("{asserv_goto} {pos_mm.x} {pos_mm.y}", channel_asserv, true)

    get()=
        <canvas id=#{id} 
                style="float: left;" 
                width="{canvas_width}" 
                height="{canvas_height}" 
                onready={on_load} onclick={on_click}>Ton navigateur supporte pas les canvas !!</canvas>
}}
