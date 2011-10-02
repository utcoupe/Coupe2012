
conf = 
{
    size=0.1
    map_width=3000.
    map_height=2000.
    robot_rayon=150.
    canvas_width=0.1*3000.
    canvas_height=0.1*2000.
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
     ctx=get_ctx()
     do Canvas.set_fill_style(ctx,{color = Color.black})
     do Canvas.fill_rect(ctx,Int.of_float(x*conf.size-conf.robot_rayon*conf.size),
                             Int.of_float(y*conf.size-conf.robot_rayon*conf.size),
                             Int.of_float(2.*conf.robot_rayon*conf.size),
                             Int.of_float(2.*conf.robot_rayon*conf.size))
     //do Canvas.begin_path(ctx)
     //do Canvas.arc_to(ctx, Int.of_float(x*conf.size), Int.of_float(y*conf.size), Int.of_float(conf.size*conf.robot_rayon), 10, 7)
     //do Canvas.fill(ctx)
     void

load_map()=
     //get canvas
     match Canvas.get(#map_bot) with
      | {some=canvas} ->
         //get context
         match Canvas.get_context_2d(canvas) with
          | {some=ctx} ->
              do Canvas.set_line_cap(ctx, {round})
              do draw_map()
              do draw_bot(500., 500., 55) 
              void
          | {none} -> jlog("error canvas 2")
         end
      | {none} -> jlog("error canvas 1")
     end

click_event(ev)=
     pos=Dom.Dimension.sub(ev.mouse_position_on_page,Dom.get_offset(#map_bot))
     newpos={x=Float.of_int(pos.x_px)/conf.size y=Float.of_int(pos.y_px)/conf.size}
     do draw_map()
     do draw_bot(newpos.x, newpos.y, 55) 
     do jlog(Debug.dump(newpos))
     void

bot_map_control()=
    <canvas id="map_bot"  width="{conf.canvas_width}" height="{conf.canvas_height}" onready={_ -> load_map()} onclick={click_event}>Ton navigateur supporte pas les canvas !!</canvas>
