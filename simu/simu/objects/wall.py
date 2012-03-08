# -*- coding: utf-8 -*-


from ..define import *
from ..engine.engineobject import EngineObjectSegment


class Wall(EngineObjectSegment):
	def __init__(self, engine, posa, posb):
		EngineObjectSegment.__init__(self,
			engine			= engine,
			colltype		= COLLTYPE_WALL,
			posA			= posa,
			posB			= posb,
			color			= "black",
			mass			= MASS_INF,
			width			= 5
		)

	def __repr__(self):
		return "Wall"
	
