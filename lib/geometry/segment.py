# -*- coding: utf-8 -*-


from .vec import Vec


class Segment:
	def __init__(self, a, b):
		self.a = Vec(a)
		self.b = Vec(b)

	def norm2(self):
		return (self.b - self.a).norm2()
	
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
		return self.a == o[0] and self.b == o[1]

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

	def intersect(self, o):
		"""
			Intersection avec un autre segment
			
				(Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
			r = -----------------------------  (eqn 1)
				(Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

				(Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
			s = -----------------------------  (eqn 2)
				(Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

			P=A+r(B-A) (intersection point)
			
			If 0<=r<=1 & 0<=s<=1, intersection exists
				r<0 or r>1 or s<0 or s>1 line segments do not intersect
		"""
		A,B,C,D = self.a, self.b, o.a, o.b

		numerateur_r = (A[1]-C[1])*(D[0]-C[0])-(A[0]-C[0])*(D[1]-C[1])
		denominateur_r = (B[0]-A[0])*(D[1]-C[1])-(B[1]-A[1])*(D[0]-C[0])
		if denominateur_r == 0: return False
		r = numerateur_r / denominateur_r
		
		numerateur_s = (A[1]-C[1])*(B[0]-A[0])-(A[0]-C[0])*(B[1]-A[1])
		denominateur_s = (B[0]-A[0])*(D[1]-C[1])-(B[1]-A[1])*(D[0]-C[0])
		if denominateur_s == 0: return False
		s = numerateur_s / denominateur_s

		return 0 <= r <= 1 and 0 <= s <= 1
		


import doctest
doctest.testfile("doctest/segment.txt")
