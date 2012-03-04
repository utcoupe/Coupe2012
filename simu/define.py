# -*- coding: utf-8 -*-

import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","lib"))

from mypyirc.ircdefine import *


FPS					= 60
PX_TO_MM			= 4

CIRCLE				= 0
POLY				= 1
WALL				= 2

COLLTYPE_DEFAULT	= 0
COLLTYPE_WALL		= 1
COLLTYPE_ROBOT		= 2
COLLTYPE_CD			= 3
COLLTYPE_ROULEAU	= 4


BLUE				= 0
RED					= 1

T_CD				= 0
T_LINGOT			= 1

class EngineObject:
	def __init__(self, t, colltype, mass, posinit, color, poly_points, radius, posa_posb, offset, custom_objects):
		"""
		@param t type d'objet CIRCLE|POLY|WALL
		@param colltype mask de collision
		@param mass la masse de l'objet
		@param posinit position initiale de l'objet
		@param color la couleur de l'objet
		@param poly_points (pour type=POLY) les points déterminants le polygon (shape) de l'objet
		@param radius (pour type=CIRCLE)
		@param posa_posb (pour type=WALL) point A, point B
		@param offset l'offset à appliquer aux points
		@param custom_objects les objets à "greffer"
		"""
		self.t = t
		self.collision_type = colltype
		self.mass = mass
		self.posinit = posinit
		self.color = color
		self.poly_points = list(poly_points)
		self.radius = radius
		self.inita,self.initb = posa_posb
		self.offset = offset
		self.custom_objects = list(custom_objects)
	
	def step(self, dt):
		pass


def mm_to_px(*args):
	"""
	@param args une liste ou un atom de valeurs ou de positions
	@param la veleur ou la liste convertie

	>>> mm_to_px(4) == int(4/PX_TO_MM)
	True

	>>> mm_to_px((4,8)) == (mm_to_px(4),mm_to_px(8))
	True

	>>> mm_to_px((4,8),(16,32)) == ((mm_to_px(4),mm_to_px(8)), (mm_to_px(16),mm_to_px(32)))
	True

	>>> mm_to_px([(4,8),(16,32)]) == mm_to_px((4,8),(16,32))
	True

	>>> mm_to_px(*[(4,8),(16,32)]) == mm_to_px((4,8),(16,32))
	True
	
	"""
	def f(a):
		if type(a) == tuple or type(a) == list:
			return mm_to_px(*a)
		else:
			return int(a / PX_TO_MM)
	if len(args) == 1:
		return f(args[0])
	else:
		return tuple(map(lambda v: f(v), args))

def px_to_mm(*args):
	"""
	@param args une liste ou un atom de valeurs ou de positions
	@param la valeur ou la liste convertie
	"""
	def f(a):
		if type(a) == tuple or type(a) == list:
			return px_to_mm(*a)
		else:
			return int(a * PX_TO_MM)
	if len(args) == 1:
		return f(args[0])
	else:
		return tuple(map(lambda v: f(v), args))




if __name__ == "__main__":
	import doctest
	doctest.testmod()



