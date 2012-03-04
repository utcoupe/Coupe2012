# -*- coding: utf-8 -*-


from ..define import *
from ..engine.engineobject import EngineObjectCircle



class Tour(EngineObjectCircle):
	def __init__(self):
		EngineObjectCircle.__init__(self,
			colltype		= COLLTYPE_WALL,
			mass			= MASS_INF,
			posinit			= mm_to_px(1500,1000),
			color			= "green",
			radius			= mm_to_px(75)
		)


	def __repr__(self):
		return "Tour"


