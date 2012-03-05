
import copy

from geometry import ConvexPoly
from .pathfinding import Node

class Area(ConvexPoly,Node):
	def __init__(self, points):
		ConvexPoly.__init__(self, points)
		Node.__init__(self)

	def __repr__(self):
		return "Area(%s, %s)" % (ConvexPoly.__repr__(self), Node.__repr__(self))
