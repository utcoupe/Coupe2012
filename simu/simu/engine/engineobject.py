
from geometry import Vec

from ..define import *


CIRCLE				= 0
POLY				= 1
WALL				= 2



class EngineObject:
	def __init__(self, *,
			t,
			engine,
			colltype		= COLLTYPE_DEFAULT,
			mass			= 1,
			posinit			= (0,0),
			color			= "black",
			offset			= (0,0),
			extension_objects	= [],
			is_extension	= False):
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
		@param extension_objects les objets à "greffer"
		"""
		if not issubclass(self.__class__, EngineObject):
			raise Exception("EngineObject est une class abstraite")
		self.t 					= t
		self.engine 			= engine
		self.collision_type 	= colltype
		self.mass 				= mass
		self.posinit 			= posinit
		self.color 				= color
		self.offset 			= offset
		self.extension_objects 	= []
		self.is_extension		= is_extension

		if not is_extension:
			self.body, self.shape = self.create_body_n_shape()
			self.shape.collision_type = colltype

		for o in extension_objects:
			self.add_body_extension(o)
		
	def add_body_extension(self, obj):
		obj.is_extension = True
		obj.body = self.body
		obj.shape = obj.create_shape(obj.body)
		obj.shape.collision_type = obj.collision_type
		self.engine.add_extension(obj)
		self.extension_objects.append(obj)
		print("add", obj.collision_type)

	def remove_body_extension(self, obj):
		self.extension_objects.remove(obj)
		self.engine.remove_extension(obj)

	def step(self, dt):
		pass

	def pos(self):
		return self.body.position

	def angle(self):
		return self.body.angle
		
	def create_body_n_shape(self):
		body = self.create_body()
		shape = self.create_shape(body)
		return body, shape

	def is_my_shape(self, shape):
		if self.shape == shape:
			return True
		else:
			for o in self.extension_objects:
				if o.is_my_shape(shape):
					return True
			return False


class EngineObjectCircle(EngineObject):
	def __init__(self, *, radius, **kwargs
			):
		self.radius = radius
		EngineObject.__init__(self, t=CIRCLE, **kwargs)

	def create_body(self):
		body = self.engine.physicsengine.create_body_circle(self.mass, self.posinit, self.radius)
		return body
	
	def create_shape(self, body):
		shape = self.engine.physicsengine.create_shape_circle(body, self.radius, self.offset)
		return shape


class EngineObjectPoly(EngineObject):
	def __init__(self, *, poly_points, **kwargs):
		self.poly_points = list(poly_points)
		EngineObject.__init__(self, t=POLY, **kwargs)

	def create_body(self):
		body,points = self.engine.physicsengine.create_body_poly(self.mass, self.posinit, self.poly_points)
		self.poly_points = points
		return body
	
	def create_shape(self, body):
		shape = self.engine.physicsengine.create_shape_poly(body, self.poly_points, self.offset)
		return shape
		

class EngineObjectSegment(EngineObject):
	def __init__(self, *, posA, posB, width, **kwargs):
		self.inita = posA
		self.initb = posB
		self.width = width
		EngineObject.__init__(self, t=WALL, **kwargs)
		
	def create_body(self):
		body = self.engine.physicsengine.create_body_segment(self.mass, self.inita, self.initb)
		return body
	
	def create_shape(self, body):
		inita = Vec(self.inita) + self.offset
		initb = Vec(self.initb) + self.offset
		print(inita)
		shape = self.engine.physicsengine.create_shape_segment(body, list(inita), list(initb), self.width)
		return shape
		
