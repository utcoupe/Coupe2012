# -*- coding: utf-8 -*-


from ..define import *
from ..engine.engineobject import EngineObjectCircle


class Cd(EngineObjectCircle):
	def __init__(self,engine,posinit,color):
		EngineObjectCircle.__init__(self,
			engine			= engine,
			colltype		= COLLTYPE_CD,
			posinit			= posinit,
			color			= color,
			radius			= mm_to_px(60)
		)

	def __repr__(self):
		return "Cd %s " % (self.posinit,)
