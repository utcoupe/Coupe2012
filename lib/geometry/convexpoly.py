

from poly import Poly


class ConvexPoly(Poly):
	def __contains__(self, o):
		"""
			Le polygon doit être défini dans le sens des aiguilles d'une montre
		"""
		if o in self.AABB:
			for edge in self.iedges():
				if edge.pos_point(o) < 0:
					return False
			return True
		else:
			return False



import doctest
doctest.testfile("doctest/convexpoly.txt")
