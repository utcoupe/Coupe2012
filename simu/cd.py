# -*- coding: utf-8 -*-

from define import *



class Cd(EngineObject):
	def __init__(self,position):
		self.posinit = position
		self.t = CIRCLE
		self.mass = 1
		self.radius = 10
		self.coef_frot = 0.5

	def step(self, dt):
		vx,vy = self.body.velocity
		self.body._set_velocity((vx*self.coef_frot, vy*self.coef_frot))

