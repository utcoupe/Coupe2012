# -*- coding: utf-8 -*-


from define import *

class Wall(EngineObject):
	def __init__(self, posa, posb):
		EngineObject.__init__(self, WALL, COLLTYPE_WALL, 0, (0,0), "black", [], 0, (posa,posb), (0,0), [])
		self.t = WALL
		self.color = "black"
		self.inita = posa
		self.initb = posb
		self.collision_type = COLLTYPE_WALL

	
