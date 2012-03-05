


import
	>>> from geometry import Segment

init
	>>> a = (0,0)
	>>> b = (4,0)
	>>> s = Segment(a,b)

equal
	>>> s == (a,b)
	True

access
	>>> s[0] == a
	True
	>>> s[1] == b
	True

iter
	>>> tuple( p for p in s) == (a,b)
	True

position d'un point
	>>> s.pos_point((2,0)) == 0
	True
	>>> s.pos_point((2,2)) > 0
	True
	>>> s.pos_point((2,-2)) < 0
	True

intersection
	>>> s1 = Segment((0,0), (4,0))
	>>> s2 = Segment((-1,-1),(5,1))
	>>> s1.intersect(s2)
	True
	>>> s1 = Segment((0,0), (4,0))
	>>> s2 = Segment((-1,-1),(5,0))
	>>> s1.intersect(s2)
	False
	>>> s1 = Segment((2, 2), (0, 0))
	>>> s2 = Segment((-1, -1), (-3, -3))
	>>> s1.intersect(s2)
	False


