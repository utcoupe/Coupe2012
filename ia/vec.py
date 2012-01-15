

from math import *
from params import *


class Vec(list):
	def __add__(self, v2):
		if len(self) != len(v2):
			raise Exception()
		else:
			return Vec(( x+y for x,y in zip(self,v2)))

	def __radd__(self, v2):
		return self + v2
	
	def __iadd__(self, v2):
		if len(self) != len(v2):
			raise Exception()
		else:
			for i,y in enumerate(v2):
				self[i] += y
		return self
	
	def __sub__(self, v2):
		if len(self) != len(v2):
			raise Exception()
		else:
			return Vec(( x-y for x,y in zip(self,v2)))

	def __rsub__(self, v2):
		return Vec(v2) - self
	
	def __isub__(self, v2):
		if len(self) != len(v2):
			raise Exception()
		else:
			for i,y in enumerate(v2):
				self[i] -= y
		return self

	def __mul__(self, a):
		return Vec(( a*x for x in self ))

	def __rmul__(self, a):
		return self * a

	def __imul__(self, a):
		for i,y in enumerate(self):
			self[i] *= a
		return self

	def __div__(self, a):
		return Vec(( x/a for x in self ))

	def __rdiv__(self, a):
		return self / a

	def __idiv__(self, a):
		for i,x in enumerate(self):
			self[i] /= a
		return self

	def __truediv__(self, a):
		return Vec(( x/a for x in self ))

	def __rtruediv__(self, a):
		return self / a

	def __itruediv__(self, a):
		for i,x in enumerate(self):
			self[i] /= a
		return self

	def __eq__(self, v2):
		if len(self) != len(v2):
			return False
		for x,y in zip(self,v2):
			if x != y:
				return False
		return True

	def normalize(self):
		self /= self.norm()
		return self

	def norm2(self):
		return sum(map(lambda x: x*x, self))

	def norm(self):
		return sqrt(self.norm2())

	def dir_to(self,other):
		if other[0] == self[0]:
			if other[1] > self[1]:
				return DOWN
			else:
				return UP
		elif other[1] == self[1]:
			if other[0] > self[0]:
				return RIGHT
			else:
				return LEFT
		else:
			v = other - self
			if v[0] == v[1]:
				if v[0] > 0:
					return DOWN_RIGHT
				else:
					return UP_LEFT
			elif v[0] == -v[1]:
				if v[0] > 0:
					return UP_RIGHT
				else:
					return DOWN_LEFT
					

	def round(self):
		for i,x in enumerate(self):
			self[i] = round(x)
		return self


def next_pos(d,pos):
	if d==UP:
		return Vec((0,-1)) + pos
	elif d==DOWN:
		return Vec((0,1)) + pos
	elif d==RIGHT:
		return Vec((1,0)) + pos
	elif d==LEFT:
		return Vec((-1,0)) + pos
	elif d==UP_LEFT:
		return Vec((-1,-1)) + pos
	elif d==UP_RIGHT:
		return Vec((1,-1)) + pos
	elif d==DOWN_LEFT:
		return Vec((-1,1)) + pos
	elif d==DOWN_RIGHT:
		return Vec((1,1)) + pos
	

import doctest
doctest.testfile("doctest/vec.txt")
