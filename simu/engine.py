# -*- coding: utf-8 -*-

from motorphysic import *
from motorgraphic import *

class Engine:
	def __init__(self):
		self.motorgraphic = MotorGraphic()
		self.motorphysic = MotorPhysic(self.motorgraphic.draw_collision)
		self.running = True

	def add(self, obj):
		self.motorgraphic.add(obj)
		self.motorphysic.add(obj)

	def start(self):
		while self.running:
			self.step()

	def step(self):
		self.motorphysic.step(1.0/60.0)
		self.running = self.motorgraphic.step()
