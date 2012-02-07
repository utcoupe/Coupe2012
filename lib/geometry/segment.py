# -*- coding: utf-8 -*-


from vec import Vec


class Segment:
	def __init__(self, a, b):
		self.a = Vec(a)
		self.b = Vec(b)

	def pos_point(self, p):
		"""
			>0 : à gauche
			<0 : à droite
			=0 : aligné
		"""
		ab = self.b - self.a
		ap = Vec(p) - self.a
		return ab[0] * ap[1] - ab[1] * ap[0]

	def __eq__(self, o):
		return len(o) == 2 and self.a == o[0] and self.b == o[1]

	def __getitem__(self, i):
		if i == 0:
			return self.a
		elif i == 1:
			return self.b
		else:
			raise Exception("Un segment ne contient que deux points, index #%s" % i)

	def __iter__(self):
		yield self.a
		yield self.b
		
	def __repr__(self):
		return "Segment(%s, %s)" % (self.a, self.b)

	def reverse(self):
		self.a,self.b = self.b,self.a


import doctest
doctest.testfile("doctest/segment.txt")
