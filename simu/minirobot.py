# -*- coding: utf-8 -*-

from pygame.locals import *

from define import *
import robot



class MiniRobot(robot.Robot):
	def __init__(self, position, color):
		robot.Robot.__init__(self,
			5,
			position,
			color,
			mm_to_px((0,0),(0,200),(200,200),(200,0))
		)

	def onEvent(self, event):
		if event.type == MOUSEBUTTONDOWN and event.button == 3: # RMB
			robot.Robot.onEvent(self, event)
