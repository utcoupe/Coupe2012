

from ..define import *


CIRCLE				= 0
POLY				= 1
WALL				= 2



class EngineObject:
	def __init__(self, *,
			t,
			colltype		= COLLTYPE_DEFAULT,
			mass			= 1,
			posinit			= (0,0),
			color			= "black",
			offset			= (0,0),
			custom_objects	= []):
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
		self.offset = offset
		self.custom_objects = list(custom_objects)
	
	def step(self, dt):
		pass

	def pos(self):
		return self.body.position

	def angle(self):
		return self.body.angle


class EngineObjectCircle(EngineObject):
	def __init__(self, *, radius, **kwargs
			):
		EngineObject.__init__(self, t=CIRCLE, **kwargs)
		self.radius = radius

class EngineObjectPoly(EngineObject):
	def __init__(self, *, poly_points, **kwargs):
		EngineObject.__init__(self, t=POLY, **kwargs)
		self.poly_points = list(poly_points)

class EngineObjectWall(EngineObject):
	def __init__(self, *, posA, posB, **kwargs):
		EngineObject.__init__(self, t=WALL, **kwargs)
		self.inita = posA
		self.initb = posB
		
