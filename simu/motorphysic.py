# -*- coding: utf-8 -*-


from define import *

import pymunk as pm
from pymunk import Vec2d


class MotorPhysic:
	def __init__(self, draw_collision):
		self.space = pm.Space()
		self.space.gravity = (0.0, 0.0)
		self.space.add_collision_handler(0, 0, None, None, draw_collision,None)

	def step(self, dt):
		self.space.step(dt)

	def add(self, obj):
		if obj.t == CIRCLE:
			inertia = pm.moment_for_circle(obj.mass, 0, obj.radius, (0,0))
			body = pm.Body(obj.mass, inertia)
			body.position = 200, 200
			shape = pm.Circle(body, obj.radius, (0,0))
		elif obj.t == POLY:
			center = pm.util.calc_center(obj.poly_points)
			moment = pm.moment_for_poly(obj.mass,obj.poly_points, Vec2d(0,0))
			body = pm.Body(obj.mass, moment)
			body.position = Vec2d(300,300)
			shape = pm.Poly(body, obj.poly_points, Vec2d(0,0))
			shape.friction = 0.5
			shape.collision_type = COLLTYPE_DEFAULT
		obj.shape = shape
		obj.body = body
		self.space.add(body, shape)
