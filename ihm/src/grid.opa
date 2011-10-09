// some aliases
i2f = Float.of_int
f2i = Int.of_float

MyMath = {{
  rad2deg(rad : float) : int=
    mod(f2i(rad*180./Math.PI),360)

  deg2rad(deg : int) : float=
    i2f(deg)*Math.PI/180.

  atan2(x : float, y : float) : float =
    if x < 0. then
        Math.atan(y/x)+Math.PI
    else
        Math.atan(y/x)
}}


/*
  Petite surcouche au canvas, pour être un peu plus libre
  Et pour gérer le repére X,Y en orthonormé (base (0,0) en bas a gauche)
*/
MyCanvas(id, width, height) = 
{{
   // Cette fonction récupére le contexte d'un canvas
   // Elle n'est pas sécurisé, et plantera les navigateurs clients qui
   // ne gérent pas les canvas, j'en suis conscient !
   // Mais la on utilise avec des navigateurs qui gérent les canvas. 
   ctx() = Option.get(Canvas.get_context_2d(Option.get(Canvas.get(#{id}))))

   // transformation x,y,a
   newx(x) = x
   newy(y) = height - y
   newa(a : int) = -a

   rectangle(couleur, x0, y0, x1, y1)=
     do Canvas.set_fill_style(ctx(),{color = couleur})
     do Canvas.fill_rect(ctx(),newx(x0),newy(y0),newx(x1),newy(y1))
     void

   arc(couleur, x, y, r, a, size) =
     ct = ctx()
     do Canvas.set_fill_style(ct,{color = couleur})
     do Canvas.begin_path(ct)
     do Canvas.arc(ct, newx(x), newy(y), r, MyMath.deg2rad(a) + size*Math.PI, MyMath.deg2rad(a) - size*Math.PI, false)
     do Canvas.close_path(ct)
     do Canvas.fill(ct)
     void

}}

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
        Scheduler.timer(200, (-> pos = position.get()
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
