

from ..pathfinding import *


class VertexNode(Node):
	def __init__(self,pos):
		Node.__init__(self)
		self.init(pos.__hash__(), pos, [])
		self.areas = []

