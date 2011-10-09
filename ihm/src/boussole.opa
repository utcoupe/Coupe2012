rad2deg(rad : float) : int=
    mod(Int.of_float(rad*180./Math.PI),360)

deg2rad(deg : int) : float=
    Float.of_int(deg)*Math.PI/180.

atan2(x : float, y : float) : float =
    if x < 0. then
        Math.atan(y/x)+Math.PI
    else
        Math.atan(y/x)

get_compas() = Option.get(Canvas.get_context_2d(Option.get(Canvas.get(#map_compas))))

draw_compas()=
    ctx = get_compas()
    do Canvas.set_fill_style(ctx,{color = Color.lightgrey})
    do Canvas.begin_path(ctx)
    do Canvas.arc(ctx, 100, 100, 100, 0., 2.*Math.PI, false)
    do Canvas.close_path(ctx)
    do Canvas.fill(ctx)
    void

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
