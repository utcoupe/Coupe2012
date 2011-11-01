# -*- coding: utf-8 -*-

import random
import math

import pymunk as pm

from define import *
from pince import *


class Robot(EngineObject):
	def __init__(self):
		self.pince = Pince()
		self.t = POLY
		
		self.mass = 10
		self.poly_points = [(0,0),(12,0),(22,12),(22,18),(18,29),(-5,29),(-9,18),(-9,12)]

		self.goals = []


	def onMouseEvent(self, x, y):
		self.goto(x,y,500)
	
	def goto(self, x, y, v):
		"""
		@param x mm
		@param y mmm
		@param v ~~
		"""
		self.goals.append((x,y,v))
	
	
	def step(self, dt):
		if self.goals:
			gx,gy,v = self.goals[0]
			x,y = self.body.position
			dx = gx - x
			dy = gy - y
			d = math.sqrt(dx**2+dy**2)
			print d, v*dt
			if d < v * dt:
				print "coucou"
				self.body._set_position((gx,gy))
				self.goals.pop(0)
				self.body._set_velocity((0,0))
			else:
				vx = dx * v / d
				vy = dy * v / d
				self.body._set_velocity((vx,vy))
			
			
