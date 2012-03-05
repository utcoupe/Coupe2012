

import
	>>> from geometry import Vec

init
	>>> v = Vec([1,2,3])
	>>> v = Vec((1,2,3))
	>>> v = Vec(range(4))

operators
	>>> v1 = Vec([1,2,3])
	>>> v2 = Vec([4,5,6])

	>>> v1 != (1,2,3)
	False
	
	>>> v1 + v2 == [5,7,9]
	True
	>>> (4,5,6) + v1 == [5,7,9]
	True

	>>> v1 - v2 == [-3,-3,-3]
	True
	>>> [5,7,9] - v1 == (4,5,6)
	True

	>>> v1 += v2
	>>> v1 == [5,7,9]
	True

	>>> v1 -= v2
	>>> v1 == [1,2,3]
	True

	>>> v1 == (1,2,3)
	True
	>>> v1 == (4,5,6)
	False

	>>> (v1 * 2) == (2,4,6)
	True
	>>> v1 *= 2
	>>> v1 == (2,4,6)
	True

	>>> (v1 / 2).round() == (1,2,3)
	True
	>>> v1 /= 2
	>>> v1 == (1,2,3)
	True

norm2 & norm
	>>> v = Vec((5,5))
	>>> v.norm2()
	50
	>>> int(v.norm())
	7

normalize
	>>> v = Vec((4,0))
	>>> v.normalize() == (1.0,.0)
	True
	>>> v = Vec((0,4))
	>>> v.normalize() == (.0,1.0)
	True



