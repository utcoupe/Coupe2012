# -*- coding: utf-8 -*-

from define import *

from engineobject import EngineObjectCircle


class Cd(EngineObjectCircle):
	def __init__(self,posinit,color):
		EngineObjectCircle.__init__(self,
			colltype		= COLLTYPE_CD,
			posinit			= posinit,
			color			= color,
			radius			= mm_to_px(60)
		)


	def __repr__(self):
		return "Cd %s " % (self.posinit,)
