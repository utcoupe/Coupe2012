# -*- coding: utf-8 -*-

import random

from define import *
from pince import *


class Robot:
	def __init__(self):
		self.pince = Pince()
		self.t = POLY
		
		self.mass = 10
		self.poly_points = [(0,0),(12,0),(22,12),(22,18),(18,29),(-5,29),(-9,18),(-9,12)]


	def setPos(self, x, y):
		self.body.position = x,y

	def onMouseEvent(self, x, y):
		self.setPos(x,y)


