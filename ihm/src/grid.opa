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
    canvas_width=f2i(conf_map.size*conf_map.width)
    canvas_height=f2i(conf_map.size*conf_map.height)
    canvas = MyCanvas(id,canvas_width,canvas_height)

    // Cette fonction dessine la map
    // Pour l'instant c'est un rectangle gris, mais a terme il faudrait dessiner
    // ou importer une image représentant mieux le terrain.
    draw_map() =
        size_bot=f2i(400.*conf_map.size)
        size_case=f2i(350.*conf_map.size)
        do canvas.rectangle(Color.black,0,0,canvas_width,canvas_height)
        do canvas.rectangle(Color.blue,0,0,size_bot,size_bot)
        do canvas.rectangle(Color.red,canvas_width-size_bot,0,canvas_width,size_bot)
        do canvas.rectangle(Color.green, 0,f2i(25.*conf_map.size)+size_bot,size_bot,canvas_height)
        do canvas.rectangle(Color.green, canvas_width-size_bot,f2i(25.*conf_map.size)+size_bot,canvas_width,canvas_height)
        do canvas.line(Color.white, canvas_width/2, 0, [(canvas_width/2, canvas_height)], 5.)
        void
         

    // Cette fonction dessine le robot
    draw_bot(x,y,a)=
        canvas.arc(Color.lightgrey, f2i(i2f(x)*conf_map.size),f2i(i2f(y)*conf_map.size),f2i(conf_map.size*conf_map.robot_rayon), a, 0.2)

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
                y=f2i(i2f(canvas_height)/conf_map.size - i2f(pos_px.y_px)/conf_map.size)}
        MyIrc.send_msg("{asserv_goto} {pos_mm.x} {pos_mm.y} 125", channel_asserv, true)

    get()=
        <canvas id=#{id} 
                style="float: left;" 
                width="{canvas_width}" 
                height="{canvas_height}" 
                onready={on_load} onclick={on_click}>Ton navigateur supporte pas les canvas !!</canvas>
}}
