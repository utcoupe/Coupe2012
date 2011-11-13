# -*- coding: utf-8 -*-


from define import *

class Wall(EngineObject):
	def __init__(self, posa, posb):
		EngineObject.__init__(self)
		self.t = WALL
		self.color = "black"
		self.inita = posa
		self.initb = posb
		self.collision_type = COLLTYPE_WALL

	
