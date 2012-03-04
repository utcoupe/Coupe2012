# -*- coding: utf-8 -*-

from pygame.locals import *

from define import *
import robot



class MiniRobot(robot.Robot):
	def __init__(self, canal_asserv, position, team):
		if team == BLUE:
			self.mouse_button = 1 # LMB
			color = 'blue'
		else:
			self.mouse_button = 3 # RMB
			color = 'red'
		robot.Robot.__init__(self,
			canal_asserv,
			team,
			position,
			5,
			color,
			mm_to_px((0,0),(0,200),(200,200),(200,0)),
			[]
		)

	def onEvent(self, event):
		if event.type == KEYDOWN and event.key == K_LSHIFT:
			self.shift_on = True
		if event.type == KEYUP and event.key == K_LSHIFT:
			self.shift_on = False
		elif self.shift_on and event.type == MOUSEBUTTONDOWN and event.button == self.mouse_button:
			p = event.pos
			print(px_to_mm(p[0],p[1]))
			self._cmd_asserv_goto(*px_to_mm(p[0],p[1],mm_to_px(1000)), id_msg=42)
