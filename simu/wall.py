# -*- coding: utf-8 -*-


from define import *
from engineobject import EngineObjectWall



class Wall(EngineObjectWall):
	def __init__(self, posa, posb):
		EngineObjectWall.__init__(self,
			colltype		= COLLTYPE_WALL,
			posA			= posa,
			posB			= posb,
			color			= "black"
		)

	def __repr__(self):
		return "Wall"
	
