# -*- coding: utf-8 -*-


from ..define import *
from .robot import Robot, MINI



class MiniRobot(Robot):
	def __init__(self, *, canal_asserv, canal_others, posinit, team, match):
		Robot.__init__(self,
			canal_asserv		= canal_asserv,
			canal_others		= canal_others,
			team				= team,
			posinit				= posinit,
			mass				= 10,
			typerobot			= MINI,
			poly_points			= mm_to_px((0,0),(184,0),(184,200),(0,200)),
		)

		self.match = match

	def onEvent(self, event):
		if not Robot.onEvent(self, event):
			pass

	def arracher_carte(self):
		self.match.arracher_carte(self.team)
