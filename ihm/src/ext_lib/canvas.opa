/*
 * @author Matthieu Guffroy
 *
 */

import stdlib.web.canvas

/*
  Petite surcouche au canvas, pour être un peu plus libre
  Et pour gérer le repére X,Y en orthonormé (base (0,0) en bas a gauche)
*/
MyCanvas(id, _width, height) = 
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
     do Canvas.fill_rect(ctx(),newx(x0),newy(y1),newx(x1),newy(y0))
     void

   arc(couleur, x, y, r, a, size) =
     ct = ctx()
     do Canvas.set_fill_style(ct,{color = couleur})
     do Canvas.begin_path(ct)
     do Canvas.arc(ct, newx(x), newy(y), r, MyMath.deg2rad(newa(a)) + size*Math.PI, MyMath.deg2rad(newa(a)) - size*Math.PI, false)
     do Canvas.close_path(ct)
     do Canvas.fill(ct)
     void

    line(couleur, x, y, l : list((int, int)), size) =
      ct = ctx()
      do Canvas.set_line_cap(ct,{round})
      do Canvas.set_stroke_style(ct,{color = couleur}) 
      do Canvas.set_line_width(ct,size)
      do Canvas.begin_path(ct)
      do Canvas.move_to(ct,x,y)
      do List.iter(( a -> (x,y) = a
                          Canvas.line_to(ct, x, y)), l)
      do Canvas.close_path(ct)
      do Canvas.stroke(ct)
      void

}}
