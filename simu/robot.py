# -*- coding: utf-8 -*-

import random

from pince import *

import pymunk as pm
from pymunk import Vec2d

class Robot:
	def __init__(self):
		self.pince = Pince()
		self.mass = 10
		self.radius = 25
		self.inertia = pm.moment_for_circle(self.mass, 0, self.radius, (0,0))
		self.body = pm.Body(self.mass, self.inertia)
		x = random.randint(115,350)
		self.body.position = x, 200
		self.shape = pm.Circle(self.body, self.radius, (0,0))
	
