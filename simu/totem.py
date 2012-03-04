# -*- coding: utf-8 -*-

from define import *
import pymunk as pm



class Totem(EngineObject):
	def __init__(self,posinit):
		EngineObject.__init__(self, POLY, COLLTYPE_WALL, pm.inf, posinit, "brown", map(lambda p: mm_to_px(*p),[(0,0),(250,0),(250,250),(0,250)]), 0, (None,None), (0,0), [])


