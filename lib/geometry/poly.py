
from vec import Vec
from aabb import AABB
from segment import Segment

class Poly:
	def __init__(self, points):
		if len(points) < 3:
			raise Exception("Un polygon doit être défini par au moins 3 points")
		self.points = list(map(lambda p: Vec(p), points))
		self.max_x = None
		self.min_x = None
		self.max_y = None
		self.min_y = None
		self.middle = None
		self.AABB = None
		self.calc_extremums()
		self.calc_middle()
		self.calc_AABB()
		
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
		self.middle = (sum_x/n, sum_y/n)

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



import doctest
doctest.testfile("doctest/poly.txt")
