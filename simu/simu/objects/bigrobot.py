# -*- coding: utf-8 -*-

from pygame.locals import *

from . import robot
from ..define import *
from ..engine.engineobject import EngineObjectPoly

class BigRobot(robot.Robot):
	
	def __init__(self, canal_asserv, position, team):
		if team == BLUE:
			self.mouse_button = 1 # LMB
			color = 'blue'
		else:
			self.mouse_button = 3 # RMB
			color = 'red'
		nbCd=0
		
		self.rouleau = EngineObjectPoly(
			colltype	= COLLTYPE_ROULEAU,
			offset		= mm_to_px(0,-85),
			color		= "orange",
			poly_points = map(lambda p: mm_to_px(*p),[(0,0),(170,0),(170,170),(0,170)])
		)
		
		robot.Robot.__init__(self,
			canal_asserv,
			team,
			position,
			10,
			color,
			mm_to_px((100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30)),
			[self.rouleau]
		)

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		if not self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			print(px_to_mm(p[0],p[1]))
			self._cmd_asserv_goto(*px_to_mm(p[0],p[1],mm_to_px(1000)), id_msg=42)
