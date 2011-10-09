/*
 * @author Matthieu Guffroy
 *
*/
/*
  Représentation d'une boussole, indiquant l'orientation du robot.
  Et permettant d'un clic, de demander au robot de se tourner.
*/

Compas = {{
   
    id = "canvas_compas"
    canvas_width=200
    canvas_height=200
    canvas = MyCanvas(id,canvas_width,canvas_height)

    // Cette fonction dessine le fond du compas
    // Pour l'instant c'est un cercle gris
    // on pourrait importer une image.
    draw_compas() =
        canvas.rectangle(Color.lightgreen,0,0,200, 200)

    //Cette fonction symbolise l'angle du robot en tracant un trait
    draw_bot(a) =
        canvas.line(Color.black, 100, 100, [(Int.of_float(100.+100.*Math.cos(MyMath.deg2rad(a))), Int.of_float(100.-100.*Math.sin(MyMath.deg2rad(a))))], 3.)

    // Au chargment
    on_load(_)=
        Scheduler.timer(200, (-> pos = Position.get()
                                 do draw_compas()
                                 do draw_bot(pos.a)
                                 void))

    on_click(ev)=
        // position en px
        pos_px=Dom.Dimension.sub(ev.mouse_position_on_page,Dom.get_offset(#{id}))
        // position en mm
        pos_mm={x=Float.of_int(pos_px.x_px)-100. y=100.-Float.of_int(pos_px.y_px)}
        do MyIrc.send_msg("{asserv_angle} {MyMath.rad2deg(MyMath.atan2(pos_mm.x,pos_mm.y))} 125", channel_asserv, true)
        void

    get()=
        <canvas id=#{id} 
                width="{canvas_width}" 
                height="{canvas_height}" 
                onready={on_load} 
                onclick={on_click}>Ton navigateur supporte pas les canvas !!</canvas>

}}
