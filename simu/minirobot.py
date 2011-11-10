# -*- coding: utf-8 -*-

from pygame.locals import *

from define import *
import robot



class MiniRobot(robot.Robot):
	def __init__(self, position, color, team):
		robot.Robot.__init__(self,
			5,
			position,
			color,
			mm_to_px((0,0),(0,200),(200,200),(200,0))
		)
		if team == 1:
			self.mouse_button = 1 # LMB
			self.canal_asserv = CANAL_ASSERV+SUFFIX_MINI
		else:
			self.mouse_button = 3 # RMB
			self.canal_asserv = CANAL_ASSERV+SUFFIX_MINI+"2"

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		elif self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			self.cmd_goto(*px_to_mm(p[0],p[1],500))
