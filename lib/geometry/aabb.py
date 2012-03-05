
from .vec import Vec


class AABB:
	def __init__(self, p, w, h):
		self.p = Vec(p)
		self.w = w
		self.h = h

	def __contains__(self, o):
		return self.p[0] <= o[0] < (self.p[0] + self.w) and self.p[1] <= o[1] < (self.p[1] + self.h)

	def __repr__(self):
		return "AABB(p=%s, w=%s, h=%s)" % (self.p, self.w, self.h)

	def aire(self):
		return self.w * self.h






