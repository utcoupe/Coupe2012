# -*- coding: utf-8 -*-


from define import *

class Wall(EngineObject):
	def __init__(self, posa, posb):
		self.t = WALL
		self.inita = posa
		self.initb = posb

	
