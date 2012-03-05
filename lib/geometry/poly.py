
from math import *

from .vec import Vec
from .aabb import AABB
from .segment import Segment

class Poly:
	def __init__(self, points=[]):
		self.points = list(map(lambda p: Vec(p), points))
		self.max_x = None
		self.min_x = None
		self.max_y = None
		self.min_y = None
		self.middle = None
		self.AABB = None
		self.color = 'white'
		self.outline = 'black'
		if len(self.points) > 2:
			self.calc()

	def move(self, v):
		for p in self.points:
			p += v
		self.calc()
		return self
	
	def move_to(self, p):
		p = Vec(p)
		v = p - self.middle
		return self.move(v)

	def round(self):
		for p in self.points:
			p.round()
		return self
			
	def adjacent(self,o):
		o_edges = o.iedges()
		for edge in self.iedges():
			edge.reverse()
			if edge in o_edges:
				return True
		return False

	def calc(self):
		self.calc_extremums()
		self.calc_middle()
		self.calc_AABB()
	
	def initFromCircle(self, p, r, n):
		p = Vec(p)
		alpha = 2*pi / n
		r_out = r / cos(alpha/2)
		teta = 0
		for i in range(n):
			A = p + (r_out * cos(teta), r_out * sin(teta))
			A.round()
			self.points.append(A)
			teta += alpha
		self.calc()
		return self

	def initFromRectangle(self, p, w, h, center=False):
		p = Vec(p)
		if center:
			p -= (w/2, h/2)
		self.points.append(p)
		self.points.append(p + (w,0))
		self.points.append(p + (w,h))
		self.points.append(p + (0,h))
		self.calc()
		return self
	
	def initFromCarre(self, p, w, center=False):
		return self.initFromRectangle(p, w, w, center)
	
	def calc_extremums(self):
		self.max_x = max(self.points, key=lambda p: p[0])[0]
		self.min_x = min(self.points, key=lambda p: p[0])[0]
		self.max_y = max(self.points, key=lambda p: p[1])[1]
		self.min_y = min(self.points, key=lambda p: p[1])[1]
		
	def calc_middle(self):
		sum_x = 0
		sum_y = 0
		for p in self.points:
			sum_x += p[0]
			sum_y += p[1]
		n = len(self.points)
		self.middle = Vec((sum_x/n, sum_y/n))

	def calc_AABB(self):
		self.calc_extremums()
		self.AABB = AABB((self.min_x,self.min_y), self.max_x - self.min_x, self.max_y - self.min_y)

	def iedges(self):
		last = self.points[0]
		for p in self.points[1:]:
			yield Segment(last, p)
			last = p
		yield Segment(self.points[-1], self.points[0])

	def __repr__(self):
		return "Poly%s" % (tuple(self.points),)


