# -*- coding: utf-8 -*-

from pygame.locals import *

from define import *
import robot
from pince import *


class BigRobot(robot.Robot):
	def __init__(self, position, color):
		robot.Robot.__init__(self,
			10,
			position,
			color,
			mm_to_px((100,0),(160,0),(280,90),(280,230),(160,320),(100,320),(0,290),(0,30))
		)
		self.pince = Pince()

	def onEvent(self, event):
		if event.type == MOUSEBUTTONDOWN and event.button == 1: # LMB
			robot.Robot.onEvent(self, event)
