# -*- coding: utf-8 -*-

from define import *
import pymunk as pm



class Totem(EngineObject):
	def __init__(self,position):
		self.t = POLY
		self.color = "black"
		self.mass = pm.inf
		self.poly_points = list(map(mm_to_px,[(0,0),(250,0),(250,250),(0,250)]))
		self.posinit = position


