


from vec import *

class Rectangle(Polygon):
	def __init__(self, x1,y1,x2,y2):
		v1 = Vec(x1,y1)
		v2 = Vec(x2,y1)
		v3 = Vec(x1,y2)
		v4 = Vec(x2,y2)
		Polygon.__init__(self,(v1,v2,v3,v4))


class Polygon:
	def __init__(self, vertex):
		self.vertex = vertex
		self.neighbors = []


