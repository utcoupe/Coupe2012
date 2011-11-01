# -*- coding: utf-8 -*-

import pymunk as pm
from pymunk import Vec2d


class MotorPhysic:
	def __init__(self, draw_collision):
		self.space = pm.Space()
		self.space.gravity = (0.0, 0.0)
		self.space.add_collision_handler(0, 0, None, None, draw_collision, None)

	def step(self, dt):
		self.space.step(dt)

	def add(self, obj):
		self.space.add(obj.body, obj.shape)
