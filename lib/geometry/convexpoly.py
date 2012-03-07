

from .poly import Poly
from .segment import *


class ConvexPoly(Poly):
	def __init__(self, points=[]):
		Poly.__init__(self, points)
		
	def set_cw(self):
		if not self.is_clockwise():
			self.points.reverse()

	def set_ccw(self):
		if self.is_clockwise():
			self.points.reverse()
	
	def is_clockwise(self):
		ab = Segment(self.points[0], self.points[1])
		c = self.points[2]
		return ab.pos_point(c) < 0
		
	def __contains__(self, o):
		"""
			Le polygon
		"""
		is_clockwise = self.is_clockwise()
		if o in self.AABB:
			for edge in self.iedges():
				if is_clockwise and edge.pos_point(o) > 0 or edge.pos_point(o) < 0:
					return False
			return True
		else:
			return False

	def intersect(self, o):
		"""
			Intersection d'un segment avec le polygon
		"""
		if o.a in self or o.b in self:
			return True
		for edge in self.iedges():
			if edge.intersect(o):
				return True
		return False


