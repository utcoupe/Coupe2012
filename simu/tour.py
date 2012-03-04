# -*- coding: utf-8 -*-

from define import *
import pymunk as pm

from engineobject import EngineObjectCircle



class Tour(EngineObjectCircle):
	def __init__(self):
		EngineObjectCircle.__init__(self,
			colltype		= COLLTYPE_WALL,
			mass			= pm.inf,
			posinit			= mm_to_px(1500,1000),
			color			= "green",
			radius			= mm_to_px(75)
		)


	def __repr__(self):
		return "Tour"


