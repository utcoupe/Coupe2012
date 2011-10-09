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
        canvas.arc(Color.lightgrey,canvas_width/2,canvas_height/2,canvas_width/2, 360, 0)

    //Cette fonction symbolise l'angle du robot en tracant un trait
    draw_bot(a) =
        canvas.line(

draw_compas_bot(a)=
    ctx=get_compas()
    do Canvas.begin_path(ctx)
    do Canvas.move_to(ctx, 100, 100)
    do Canvas.line_to(ctx, Int.of_float(100.+100.*Math.cos(deg2rad(a))), Int.of_float(100.-100.*Math.sin(deg2rad(a))))
    do Canvas.close_path(ctx)
    do Canvas.stroke(ctx)
    void

load_compas()=
    do Scheduler.timer(200, (-> pos = Position.get()
                                 do draw_compas()
                                 do draw_compas_bot(pos.a)
                                 void))
     match Canvas.get(#map_compas) with
      | {some=canvas} ->
         //get context
         match Canvas.get_context_2d(canvas) with
          | {some=ctx} ->
              do Canvas.set_line_cap(ctx, {round})
              do draw_compas() 
              void
          | {none} -> jlog("error canvas 2")
         end
      | {none} -> jlog("error canvas 1")
     end
click_compas_event(ev)=
     pos=Dom.Dimension.sub(ev.mouse_position_on_page,Dom.get_offset(#map_compas))
     newpos={x=Float.of_int(pos.x_px)-100. y=100.-Float.of_int(pos.y_px)}
     do MyIrc.send_msg("{asserv_angle} {rad2deg(atan2(newpos.x,newpos.y))}", channel_asserv, true)
     void

compas()=
    <canvas id="map_compas" width="200" height="200" onready={_ -> load_compas()} onclick={click_compas_event}>Ton navigateur supporte pas les canvas !!</canvas>
