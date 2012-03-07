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


	def create_shape_circle(self, body, radius, offset):
		return pm.Circle(body, radius, offset)
		
	def create_shape_poly(self, body, points, offset):
		return pm.Poly(body, points, offset)
		
	def create_shape_wall(self, body, p1, p2):
		return pm.Segment(body, p1, p2, 0.0)


	def create_body_circle(self, mass, posinit, radius):
		inertia = pm.moment_for_circle(mass, 0, radius, (0,0))
		body = pm.Body(mass, inertia)
		body.position = Vec2d(posinit)
		return body

	def create_body_poly(self, mass, posinit, points):
		center = pm.util.calc_center(points)
		points = pm.util.poly_vectors_around_center(points)
		moment = pm.moment_for_poly(mass,points, Vec2d(0,0))
		body = pm.Body(mass, moment)
		body.position = Vec2d(posinit)
		return body, points

	def create_body_wall(self):
		body = pm.Body(pm.inf, pm.inf)
		return body
		
	def add(self, obj):
		for o in obj.custom_objects:
			self.space.add(o.shape)
		self.space.add(obj.body)
		self.space.add(obj.shape)
		self.objects.append(obj)

	def remove(self, obj):
		for o in obj.custom_objects:
			self.remove(o)
		self.space.remove(obj.shape, obj.body)
		self.objects.remove(obj)


		
