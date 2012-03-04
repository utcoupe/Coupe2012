# -*- coding: utf-8 -*-

from define import *
import pymunk as pm



class Tour(EngineObject):
	def __init__(self):
		EngineObject.__init__(self, CIRCLE, COLLTYPE_WALL, pm.inf, mm_to_px(1500,1000), "green", [], mm_to_px(75), (None,None), (0,0), [])


