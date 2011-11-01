# -*- coding: utf-8 -*-

from motorphysic import *
from motorgraphic import *

class Engine:
	def __init__(self):
		self.motorgraphic = MotorGraphic()
		self.motorphysic = MotorPhysic(self.motorgraphic.draw_collision)
		self.running = True
		self.objects = []

	def add(self, obj):
		self.objects.append(obj)
		self.motorgraphic.add(obj)
		self.motorphysic.add(obj)

	def start(self):
		while self.running:
			self.step()

	def step(self):
		dt = 1.0/60.0
		for o in self.objects:
			o.step(dt)
		self.motorphysic.step(dt)
		self.running = self.motorgraphic.step()
