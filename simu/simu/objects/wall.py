# -*- coding: utf-8 -*-


from ..define import *
from ..engine.engineobject import EngineObjectWall


class Wall(EngineObjectWall):
	def __init__(self, engine, posa, posb):
		EngineObjectWall.__init__(self,
			engine			= engine,
			colltype		= COLLTYPE_WALL,
			posA			= posa,
			posB			= posb,
			color			= "black",
			mass			= MASS_INF
		)

	def __repr__(self):
		return "Wall"
	
