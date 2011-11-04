# -*- coding: utf-8 -*-

from define import *
import pymunk as pm



class Tour(EngineObject):
	def __init__(self):
		self.posinit = mm_to_px((1500,1000))
		self.t = CIRCLE
		self.mass = pm.inf
		self.radius = mm_to_px(150)
		self.color = "green"
		self.collision_type = COLLTYPE_WALL


