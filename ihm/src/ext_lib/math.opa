/*
 * @author Matthieu Guffroy
 * Ceci est une extension de la lib Math
 */

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
