# -*- coding: utf-8 -*-

from pygame.locals import *

from define import *
import robot
from pince import *

class Rouleau(EngineObject):
	def __init__(self, offset):
		EngineObject.__init__(self)
		self.t = POLY
		self.color = "orange"
		self.poly_points = list(map(lambda p: mm_to_px(*p),[(0,0),(170,0),(170,170),(0,170)]))
		self.offset = offset
		self.collision_type = COLLTYPE_ROULEAU


class BigRobot(robot.Robot):
	
	def __init__(self, canal_asserv, position, team):
		if team == BLUE:
			self.mouse_button = 1 # LMB
			color = 'blue'
		else:
			self.mouse_button = 3 # RMB
			color = 'red'
		nbCd=0
		self.rouleau = Rouleau(mm_to_px(0,-85))
		robot.Robot.__init__(self,
			canal_asserv,
			team,
			position,
			10,
			color,
			mm_to_px((100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30)),
			self.rouleau
		)
		self.pince = Pince()

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		if not self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			self._cmd_asserv_goto(*px_to_mm(p[0],p[1],500), id_msg=42)
