# -*- coding: utf-8 -*-

from define import *
from motorphysic import *
from motorgraphic import *

class Engine:
	def __init__(self, stop_irc):
		self.motorgraphic = MotorGraphic()
		self.motorphysic = MotorPhysic()
		self.motorphysic.add_collision_handler(COLLTYPE_ROBOT, COLLTYPE_WALL, self.motorgraphic.draw_collision)
		self.running = True
		self.objects = []
		self.stop_irc = stop_irc

	def add(self, obj):
		self.objects.append(obj)
		self.motorgraphic.add(obj)
		self.motorphysic.add(obj)

	def start(self):
		while self.running:
			self.step()
		self.stop_irc()

	def step(self):
		dt = 1.0/float(FPS)
		self.motorphysic.step(dt)
		for o in self.objects:
			o.step(dt)
		self.running = self.motorgraphic.step()

		
