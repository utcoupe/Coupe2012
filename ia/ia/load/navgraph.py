

from vec import *
from collisionmatrix import *

class NavGraph:
	def __init__(self):
		self.collision_matrix = CollisionMatrix()
		self.polys = []

	def generate_from_img(self, filename, scale=1):
		self.collision_matrix.load_img(filename, scale)
		print ("warning generate_from_img doesn't work yet")

	def generate_from_xml(self, filename, scale=1):
		self.collision_matrix.load_xml(filename, scale)
		self._generate()

	def generate_from_polys_xml(self, filename, scale=1):
		

	def find_nearest_vertice(self, vertice, polys):
		nearest = polys.vertex[0]
		min_dist = (nearest - vertice).norm2()
		for v in polys.vertex[1:]:
			dist = (v - vertice).norm2()
			if dist < min_dist:
				min_dist = dist
				nearest = v
		return nearest
		
