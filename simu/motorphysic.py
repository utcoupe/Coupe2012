# -*- coding: utf-8 -*-


from define import *

import pymunk as pm
from pymunk import Vec2d


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

	def add(self, obj):
		if obj.t == CIRCLE:
			inertia = pm.moment_for_circle(obj.mass, 0, obj.radius, (0,0))
			body = pm.Body(obj.mass, inertia)
			body.position = obj.posinit[0], obj.posinit[1]
			shape = pm.Circle(body, obj.radius, (0,0))
		elif obj.t == POLY:
			center = pm.util.calc_center(obj.poly_points)
			obj.poly_points = pm.util.poly_vectors_around_center(obj.poly_points)
			moment = pm.moment_for_poly(obj.mass,obj.poly_points, Vec2d(0,0))
			body = pm.Body(obj.mass, moment)
			body.position = obj.posinit[0], obj.posinit[1]
			shape = pm.Poly(body, obj.poly_points, Vec2d(0,0))
			shape.friction = 0.5
		elif obj.t == WALL:
			body = pm.Body(pm.inf, pm.inf)
			shape = pm.Segment(body, obj.inita, obj.initb, 0.0)
		shape.collision_type = obj.collision_type
		obj.shape = shape
		obj.body = body
		self.space.add(body, shape)
		self.objects.append(obj)
