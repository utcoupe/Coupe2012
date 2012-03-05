


import
	>>> from geometry import ConvexPoly

init
	>>> poly = ConvexPoly(((0,0),(4,0),(2,2)))

point dans le poly
	>>> (2,1) in poly
	True
	>>> (-1,4) in poly
	False

isclockwise
	>>> poly.is_clockwise()
	False

intersection
	>>> from geometry import Segment
	>>> poly.intersect(Segment((-1,-1), (-3,-3)))
	False
	>>> poly.intersect(Segment((-1,-1), (5,5)))
	True
	
