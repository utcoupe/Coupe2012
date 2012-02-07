
from geometry.vec import Vec
from params import *

class IAVec(Vec):
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
					


def next_pos(d,pos):
	if d==UP:
		return IAVec((0,-1)) + pos
	elif d==DOWN:
		return IAVec((0,1)) + pos
	elif d==RIGHT:
		return IAVec((1,0)) + pos
	elif d==LEFT:
		return IAVec((-1,0)) + pos
	elif d==UP_LEFT:
		return IAVec((-1,-1)) + pos
	elif d==UP_RIGHT:
		return IAVec((1,-1)) + pos
	elif d==DOWN_LEFT:
		return IAVec((-1,1)) + pos
	elif d==DOWN_RIGHT:
		return IAVec((1,1)) + pos
	
