# -*- coding: utf-8 -*-

from define import *



class Cd(EngineObject):
	def __init__(self,position,color):
		EngineObject.__init__(self)
		self.posinit = position
		self.t = CIRCLE
		self.mass = 1
		self.radius = mm_to_px(60)
		self.color = color
		self.collision_type = COLLTYPE_CD

			
