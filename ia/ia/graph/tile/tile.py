

from geometry import ConvexPoly,Vec
from ..pathfinding import Node



class Tile(ConvexPoly,Node):
	def __init__(self, p, w):
		Node.__init__(self)
		ConvexPoly.__init__(self)
		self.initFromCarre(Vec(p) * w, w)
		self.w = w
	
	def __repr__(self):
		return "Tile(%s, %s)" % (ConvexPoly.__repr__(self), Node.__repr__(self))

