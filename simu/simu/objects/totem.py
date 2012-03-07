# -*- coding: utf-8 -*-



from ..define import *
from ..engine.engineobject import EngineObjectPoly


class Totem(EngineObjectPoly):
	def __init__(self,engine,posinit):
		EngineObjectPoly.__init__(self,
			engine			= engine,
			colltype		= COLLTYPE_WALL,
			posinit			= posinit,
			color			= "brown",
			mass			= MASS_INF,
			poly_points		= map(lambda p: mm_to_px(*p),[(0,0),(250,0),(250,250),(0,250)])
		)


	def __repr__(self):
		return "Totem"
