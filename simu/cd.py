# -*- coding: utf-8 -*-

from define import *



class Cd(EngineObject):
	def __init__(self,posinit,color):
		EngineObject.__init__(self, CIRCLE, COLLTYPE_CD, 1, posinit, color, [], mm_to_px(60), (None,None), (0,0), [])

			
