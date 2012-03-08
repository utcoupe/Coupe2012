# -*- coding: utf-8 -*-


from ..define import *
from .robot import Robot, MINI
from ..engine.engineobject import EngineObjectPoly

class Palette(EngineObjectPoly):
	def __init__(self, angle):
		pass

class MiniRobot(Robot):
	def __init__(self, *, engine, canal_asserv, canal_others, posinit, team, match):
		Robot.__init__(self,
			engine 				= engine,
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
			if self._event_concerns_me(event):
				if KEYDOWN == event.type:
					if KEY_ARRACHE == event.key:
						self._cmd_others_arrachercarte(id_msg=42)
						return True

	def _cmd_others_arrachercarte(self, **kwargs):
		self.match.arracher_carte(self.team)
		self.send_canal_others(kwargs['id_msg'], 1)
