# -*- coding: utf-8 -*-

from pygame.locals import *

from define import *
import robot
from pince import *


class BigRobot(robot.Robot):
	def __init__(self, position, color, team):
		robot.Robot.__init__(self,
			10,
			position,
			color,
			mm_to_px((100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30))
		)
		self.pince = Pince()
		if team == 1:
			self.mouse_button = 1 # LMB
			self.canal_asserv = CANAL_ASSERV
		else:
			self.mouse_button = 3 # RMB
			self.canal_asserv = CANAL_ASSERV+"2"

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		if not self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			self.cmd_goto(*px_to_mm(p[0],p[1],500))
