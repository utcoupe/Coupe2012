
conf = 
{
    size=0.2
    map_width=3000.
    map_height=2100.
    robot_rayon=150.
    canvas_width=0.2*3000.
    canvas_height=0.2*2100.
}

get_ctx() = Option.get(Canvas.get_context_2d(Option.get(Canvas.get(#map_bot))))

// This function draw the map 
draw_map()=
    ctx = get_ctx()
    do Canvas.set_fill_style(ctx,{color = Color.lightgrey})
    do Canvas.fill_rect(ctx,0,0,Int.of_float(conf.canvas_width),Int.of_float(conf.canvas_height))
    void

// This function draw the bot
draw_bot(x, y, a)=
     y=conf.map_height-y
     a=-a
     ctx=get_ctx()
     PI = 3.14
     a = a * 2.*PI/360.
     do Canvas.set_fill_style(ctx,{color = Color.black})
     do Canvas.begin_path(ctx)
     do Canvas.arc(ctx, Int.of_float(x*conf.size), Int.of_float(y*conf.size), Int.of_float(conf.size*conf.robot_rayon), a + 0.2*PI, a - 0.2*PI, false)
     do Canvas.close_path(ctx)
     do Canvas.fill(ctx)
     void

load_map()=
     //get canvas
     do Scheduler.timer(200, (-> pos = position.get()
                                 do draw_map()
                                 do draw_bot(Float.of_int(pos.x),
                                             Float.of_int(pos.y),
                                             Float.of_int(pos.a))
                                 void))
     match Canvas.get(#map_bot) with
      | {some=canvas} ->
         //get context
         match Canvas.get_context_2d(canvas) with
          | {some=ctx} ->
              do Canvas.set_line_cap(ctx, {round})
              do draw_map()
              void
          | {none} -> jlog("error canvas 2")
         end
      | {none} -> jlog("error canvas 1")
     end

click_event(ev)=
     pos=Dom.Dimension.sub(ev.mouse_position_on_page,Dom.get_offset(#map_bot))
     newpos={x=Float.of_int(pos.x_px)/conf.size y=conf.map_height - Float.of_int(pos.y_px)/conf.size}
     do draw_map()
     //do draw_bot(newpos.x, newpos.y, 95.)
     do Irc_Asserv.send_msg("goto {Int.of_float(newpos.x)} {Int.of_float(newpos.y)}", true)
     //do jlog(Debug.dump(newpos))
     void

bot_map_control()=
    <canvas id="map_bot" style="float: left;" width="{conf.canvas_width}" height="{conf.canvas_height}" onready={_ -> load_map()} onclick={click_event}>Ton navigateur supporte pas les canvas !!</canvas>
