# -*- coding: utf-8 -*-



import pymunk as pm
from pymunk import Vec2d

from ..define import *
from .engineobject import *

class MotorPhysic:
	""" unité : mm """
	def __init__(self):
		self.space = pm.Space()
		self.space.gravity = (0.0, 0.0)
		self.objects = []
		self.coef_frot = 0.85

	def add_collision_handler(self, colltype1, colltype2, f):
		self.space.add_collision_handler(colltype1, colltype2, None, None, f, None)
	
	def step(self, dt):
		self.space.step(dt)
		for o in self.objects:
			vx,vy = o.body.velocity
			o.body._set_velocity((vx*self.coef_frot, vy*self.coef_frot))

	def create_shape(self, obj, body):
		if obj.t == CIRCLE:
			shape = pm.Circle(body, obj.radius, obj.offset)
		elif obj.t == POLY:
			shape = pm.Poly(body, obj.poly_points, obj.offset)
		elif obj.t == WALL:
			shape = pm.Segment(body, obj.inita, obj.initb, 0.0)
		else:
			raise Exception("MotorPhysic.create_shape : objet type '%s' doesn't exist"%obj.t)
		return shape


	def create_body(self, obj):
		if obj.t == CIRCLE:
			inertia = pm.moment_for_circle(obj.mass, 0, obj.radius, (0,0))
			body = pm.Body(obj.mass, inertia)
			body.position = obj.posinit[0], obj.posinit[1]
		elif obj.t == POLY:
			center = pm.util.calc_center(obj.poly_points)
			obj.poly_points = pm.util.poly_vectors_around_center(obj.poly_points)
			moment = pm.moment_for_poly(obj.mass,obj.poly_points, Vec2d(0,0))
			body = pm.Body(obj.mass, moment)
			body.position = obj.posinit[0], obj.posinit[1]
		elif obj.t == WALL:
			body = pm.Body(pm.inf, pm.inf)
		else:
			raise Exception("MotorPhysic.create_body : objet type '%s' doesn't exist"%obj.t)
		return body
		
	
	def add(self, obj, body=None):
		if not body:
			body = self.create_body(obj)
			self.space.add(body)
		shape = self.create_shape(obj, body)
		shape.collision_type = obj.collision_type
		obj.shape = shape
		obj.body = body
		self.space.add(shape)
		self.objects.append(obj)
		for o in obj.custom_objects:
			self.add(o, body)
		return body, shape

	def remove(self, obj):
		for o in obj.custom_objects:
			self.remove(o)
		self.space.remove(obj.shape, obj.body)
		self.objects.remove(obj)


		
