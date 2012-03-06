# -*- coding: utf-8 -*-


from ..define import *
from ..engine.engineobject import EngineObjectPoly


class Lingo(EngineObjectPoly):
	def __init__(self,posinit):
		EngineObjectPoly.__init__(self,
			colltype		= COLLTYPE_LINGO,
			posinit			= posinit,
			color			= "yellow",
			poly_points		= map(lambda p: mm_to_px(*p),[(0,0),(100,0),(100,200),(0,200)])
		)

	def __repr__(self):
		return "Cd %s " % (self.posinit,)
