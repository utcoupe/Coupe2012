# -*- coding: utf-8 -*-

from define import *



class Cd(EngineObject):
	def __init__(self,position):
		self.posinit = position
		self.t = CIRCLE
		self.mass = 1
		self.radius = mm_to_px(60)


